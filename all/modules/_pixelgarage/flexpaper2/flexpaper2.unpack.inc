<?php
/**
 * This file contains all functions needed to unpack and install a locally published PDF (locally with FlexPaper Zine)
 * The published file is uploaded as an archive in a format that has to be supported by Drupal.
 *
 */

/* -----------------------------------------------------------------
 *  Batch publishing functions
 * ----------------------------------------------------------------- */

/**
 * Batch callback: Unpacks and verifies an uploaded published PDF archive.
 *
 * This function assumes that the provided file points to an archive that
 * contains a published PDF file structure of the Flexpaper Zine application.
 *
 * @param string $file
 *   The file object of the uploaded archive.
 * @param array $context
 *   Reference to an array used for Batch API storage.
 */
function flexpaper2_extract_archive_batch_process($file, &$context) {
  // This is here to show the user that we are extracting the archive.
  if (empty($context['sandbox'])) {
    $context['sandbox']['started'] = TRUE;
    $context['message'] = t('Extracting published PDF archive %file ...', array('%file' => $file->uri));
    $context['finished'] = 0;
    return;
  }

  // extract uploaded archive to extract directory and validate content
  $archive_path = drupal_realpath($file->uri);
  $tmp_dir = _flexpaper2_extract_directory();

  try {
    $top_dir = _flexpaper2_archive_extract($archive_path, $tmp_dir);
    $extracted_dir = drupal_realpath($tmp_dir) . '/' . $top_dir;
  }
  catch (Exception $e) {
    _flexpaper2_batch_error($e->getMessage(), null,  $context);
    return;
  }

  //
  // prepare files publish directory and check archive content
  $publish_directory = flexpaper2_get_files_folder_path($file);
  if(!file_prepare_directory($publish_directory, FILE_CREATE_DIRECTORY)) {
    watchdog('flexpaper2', 'Error in the attempt to create %directory folder. Check flexpaper folder permissions.',
      array('%directory' => $publish_directory), WATCHDOG_ERROR);
    $context['message'] = t('Error in the attempt to create %directory folder.',
      array('%directory' => $publish_directory));
    $context['results']['create_folder_error'] = TRUE;
    $context['finished'] = 1;
    return;
  }
  $files = scandir($extracted_dir . '/docs');
  if (!$files) {
    $msg = t('No published files found in archive. Check flexpaper archive!');
    _flexpaper2_batch_error($msg, $publish_directory, $context);
    return;
  }

  // copy and rename all relevant files in the extracted directory to the publish directory
  $pos = strpos($file->filename, '.zip');
  $filename = substr($file->filename, 0, $pos);
  $extensions = array('.swf', '.pdf', '_thumb.jpg', '.jpg', '.js');
  foreach ($extensions as $ext) {
    $pattern = sprintf('/\%s$/', $ext);
    $files = file_scan_directory($extracted_dir . '/docs', $pattern, array('key' => 'name'));

    foreach ($files as $uri => $obj) {
      $destination = $publish_directory . '/' . str_replace($filename, $file->fid, $obj->filename);
      if (!file_unmanaged_move($obj->uri, $destination, FILE_EXISTS_REPLACE)) {
        $msg = t('Error in the attempt to copy %file to publish folder.', array('%file' => $uri));
        _flexpaper2_batch_error($msg, $publish_directory, $context);
        return;
      }
    }
  }

  // copy index.html file
  file_unmanaged_copy($extracted_dir . '/index.html', $publish_directory, FILE_EXISTS_REPLACE);

  // copy pdf file if available
  $pdfpath = $extracted_dir . '/' . $filename . '.pdf';
  if (file_exists($pdfpath)) {
    file_unmanaged_copy($pdfpath, $publish_directory, FILE_EXISTS_REPLACE);
  }

  // delete temporary extract directory
  if (file_exists($tmp_dir)) {
    file_unmanaged_delete_recursive($tmp_dir);
  }

  // finished with success.
  $context['finished'] = 1;
}

/**
 * Batch callback: Performs actions when the archive extraction batch is completed.
 *
 * @param $success
 *   TRUE if the batch operation was successful, FALSE if there were errors.
 * @param $results
 *   An associative array of results from the batch operation.
 */
function flexpaper2_extract_archive_batch_finished($success, $results) {
  if(isset($results['create_folder_error'])) {
    $message = t('An error occurred while creating the folder for the extracted flexpaper files. See log messages for details.');
    drupal_set_message($message, 'error');
    drupal_goto('');
  }
  if (!$success || (isset($results['error']) && $results['error'])) {
    if (isset($results['error_msg'])) {
      $message = $results['error_msg'];
    }
    else {
      $message = t('An error occurred while publishing pdf files. See log messages for details.');
    }
    drupal_set_message($message, 'error');

  }
  else {
    $message = t('Extracting published pdf archive was succesfully completed.');
    drupal_set_message($message);
  }
}

/**
 * Parses the index file in the uploaded archive and returns the found Flexpaper viewer settings
 * as associative array.
 *
 * @param $file object
 *  The file object of the uploaded archive.
 * @return array
 *  An associative array containing the parsed FlexPaperViewer settings.
 */
function flexpaper2_parse_settings_file($file) {
  $js_settings = array();
  $settings_file = flexpaper2_get_files_folder_path($file) . '/index.html';
  $content = file_get_contents($settings_file);

  // get settings part in file
  $pos1 = strpos($content, '.FlexPaperViewer(') + strlen('.FlexPaperViewer(');
  $content = substr($content, $pos1);
  $pos2 = strpos($content, ');');
  $content = trim(substr($content, 0, $pos2));

  // extract FlexPaper viewer settings
  $lines = explode(PHP_EOL, $content);
  foreach ($lines as $line) {
    $excludes = array('{ config', 'SWFFile', 'IMGFiles', 'PDFFile', 'JSONFile', 'ThumbIMGFiles', 'UIConfig', 'WMode', 'key', 'localeChain');
    if (strpos($line, ':')) {
      $setting = explode(':', $line);
      $key = trim($setting[0]);
      if (!in_array($key, $excludes)) {
        $value = trim(trim($setting[1]), "',");
        switch($value) {
          case 'true':
            $value = 1;
            break;
          case 'false':
            $value = 0;
            break;
        }
        $js_settings[$key] = $value;
      }
    }
  }

  // replace UIConfig file
  global $base_url;
  $path = $base_url . '/' . libraries_get_path('flexpaper');
  $js_settings['UIConfig'] = $path . '/UIConfig_standard.pdf.xml';


  return $js_settings;
}



/* -----------------------------------------------------------------
 *  Local helper functions
 * ----------------------------------------------------------------- */

function _flexpaper2_batch_error($msg, $dir, &$context) {
  // set results in context
  $context['results']['error_msg'] = $msg;
  $context['results']['error'] = TRUE;
  $context['finished'] = 1;

  // delete publish directory, if already created
  if ($dir && file_exists($dir)) {
    file_unmanaged_delete_recursive($dir);
  }
}

/**
 * Returns a short unique identifier for this Drupal installation.
 *
 * @return
 *   An eight character string uniquely identifying this Drupal installation.
 */
function _flexpaper2_unique_identifier() {
  $id = &drupal_static(__FUNCTION__, '');
  if (empty($id)) {
    $id = substr(hash('sha256', drupal_get_hash_salt()), 0, 8);
  }
  return $id;
}

/**
 * Returns the directory where update archive files should be extracted.
 *
 * @param $create
 *   (optional) Whether to attempt to create the directory if it does not
 *   already exist. Defaults to TRUE.
 *
 * @return
 *   The full path to the temporary directory where update file archives should
 *   be extracted.
 */
function _flexpaper2_extract_directory($create = TRUE) {
  $directory = &drupal_static(__FUNCTION__, '');
  if (empty($directory)) {
    $directory = 'temporary://flexpaper-extraction-' . _update_manager_unique_identifier();
    if ($create && !file_exists($directory)) {
      mkdir($directory);
    }
  }
  return $directory;
}

/**
 * Unpacks a uploaded archive file.
 *
 * @param string $file
 *   The filename of the archive you wish to extract.
 * @param string $directory
 *   The directory you wish to extract the archive into.
 *
 * @return string
 *   The extracted top directory name.
 *
 * @throws Exception
 */
function _flexpaper2_archive_extract($file, $directory) {
  $archiver = archiver_get_archiver($file);
  if (!$archiver) {
    throw new Exception(t('Cannot extract %file, not a valid archive.', array ('%file' => $file)));
  }

  // List archive files and make sure the extract location is empty
  $files = $archiver->listContents();
  if (!$files) {
    throw new Exception(t('Provided archive %file contains no files.', array ('%file' => $file)));
  }
  else if (file_exists($directory)) {
    file_unmanaged_delete_recursive($directory);
  }

  // extract archive to temporary directory
  $archiver->extract($directory);

  // Some archivers list the first file as the directory (i.e.,DIR/)
  // and others list an actual file (i.e., DIR/README.TXT).
  $dir = strtok($files[0], '/\\');

  return $dir;
}


