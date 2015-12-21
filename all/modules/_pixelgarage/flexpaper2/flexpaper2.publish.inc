<?php
/**
 * This file contains all functions needed to integrate the Flexpaper server publishing of an
 * uploaded PDF file.
 */


/* -----------------------------------------------------------------
 *  Batch publishing functions
 * ----------------------------------------------------------------- */

/**
 * Flexpaper processor - Batch API process callback.
 */
function flexpaper2_publish_files_batch_process($file, &$context) {
  $fid = $file->fid;
  $context['results']['fid'] = $fid;
  $pdf_path = drupal_realpath($file->uri);
  $files_directory = drupal_realpath(flexpaper2_get_files_folder_path($file));

  // create published files directory
  if(!file_prepare_directory($files_directory, FILE_CREATE_DIRECTORY)) {
    watchdog('flexpaper2', 'Error in the attempt to create %directory folder. Check flexpaper folder permissions.',
      array('%directory' => $files_directory), WATCHDOG_ERROR);
    $context['message'] = t('Error in the attempt to create %directory folder.',
      array('%directory' => $files_directory));
    $context['results']['create_folder_error'] = TRUE;
    return;
  }

  // check if server publishing is available
  if (!flexpaper2_server_publishing_available()) {
    // copy pdf file to files directory (only html5)
    copy($pdf_path, $files_directory . "/" . $fid . '.pdf');
    $context['finished'] = 1;
    return;
  }

  // initialize progress info, if possible
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $max_pages = _flexpaper2_get_pdf_pages_number($pdf_path);

    if ($max_pages != 0) {
      // It means that our function was able to count number of pages.
      // So, we can show progress info for user.
      $context['sandbox']['progress_info'] = TRUE;
      $context['sandbox']['max'] = $max_pages;
      $context['sandbox']['last_page'] = FALSE;
      $context['sandbox']['success'] = FALSE;
    }
    else {
      // We don't have number of pages. One attempt to make all operations.
      $context['sandbox']['progress_info'] = FALSE;
    }
  }

  if ($context['sandbox']['progress_info']) {
    // publish with progress info
    if (!$context['sandbox']['last_page']) {
      $first_page_number = $context['sandbox']['progress'] + 1;
      $last_page_number = min($context['sandbox']['progress'] + 3, $context['sandbox']['max']);
      $context['sandbox']['success'] = _flexpaper2_generate_swf_files($pdf_path, $files_directory, $fid, $first_page_number, $last_page_number);
      $context['sandbox']['progress'] = $last_page_number;
    }
    if ($context['sandbox']['success']) {
      // update progress info
      if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
        $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        $context['message'] = t("@progress of @max files are processed", array(
          '@progress' => $context['sandbox']['progress'],
          '@max' => $context['sandbox']['max'],
        ));
      }
      elseif ($context['sandbox']['last_page'] == FALSE) {
        // last page processing
        $context['finished'] = ($context['sandbox']['max'] - 0.1) / $context['sandbox']['max'];
        $context['sandbox']['last_page'] = TRUE;
        $context['message'] = t('Publishing is finishing...');
      }
      else {
        // complete publishing
        // create pdf pages (html5)
        copy($pdf_path, $files_directory . "/" . $fid . '.pdf');
        $success1 = _flexpaper2_exec_cmd(_pdftk_cmd($pdf_path, $files_directory, $fid));

        // Create json file as last file
        $success2 = _flexpaper2_exec_cmd(_pdf2json_cmd($pdf_path, $files_directory, $fid));
        if ($success1 && $success2) {
          $context['message'] = t("@max of @max pages are processed", array('@max' => $context['sandbox']['max']));
        }
        else {
          $context['results']['error'] = TRUE;
        }
        $context['finished'] = 1;
      }
    }
    else {
      $context['finished'] = 1;
      $context['results']['error'] = TRUE;
    }
  }
  else {
    // publish all at once (no progress info)
    $success1 = _flexpaper2_generate_swf_files($pdf_path, $files_directory, $fid);
    $success2 = _flexpaper2_exec_cmd(_pdftk_cmd($pdf_path, $files_directory, $fid));
    $success3 = _flexpaper2_exec_cmd(_pdf2json_cmd($pdf_path, $files_directory, $fid));
    if(!($success1 && $success2 && $success3)) {
      $context['results']['error'] = TRUE;
    }
    $context['finished'] = 1;
  }
}

/**
 * Flexpaper publishing batch 'finished' callback.
 */
function flexpaper2_publish_files_batch_finish($success, $results, $operations) {
  if(isset($results['create_folder_error'])) {
    $message = t('An error occurred while creating the folder for the flexpaper files. See log messages for details.');
    drupal_set_message($message, 'error');
    drupal_goto('');
  }
  if (!$success || (isset($results['error']) && $results['error'])) {
    $message = t('An error occurred while processing generating flexpaper files. See log messages for details.');
    drupal_set_message($message, 'error');
  }
  else {
    $message = t('Publishing pdf was succesfully completed.');
    drupal_set_message($message);
  }
}

/* -----------------------------------------------------------------
 *  Local functions
 * ----------------------------------------------------------------- */

/**
 * Publish pdf in different formats (swf and png pages) for the given range of pdf pages.
 * If start page number is not specified, consider that we need conversion for all pages.
 * Returns FALSE if there was an error.
 */
function _flexpaper2_generate_swf_files($pdf_path, $files_directory, $name_root, $start_page_number = 0, $end_page_number = 0) {
  // create flash and html4 files
  if ($start_page_number > 0) {
    $success = _flexpaper2_exec_cmd(_pdf2swf_cmd($pdf_path, $files_directory, $name_root, $start_page_number, $end_page_number));
    if (!$success) return false;

    // If there was no error, then proceed with creation of png files.
    for ($i = $start_page_number; $i <= $end_page_number; $i++) {
      $filename = $name_root . '_' . $i;
      if ( !_flexpaper2_exec_cmd(_swfrender_cmd($files_directory, $filename)) ) return false;
    }
  }
  else {
    $success = _flexpaper2_exec_cmd(_pdf2swf_cmd($pdf_path, $files_directory, $name_root));
    if (!$success) return false;

    // If there was no error, then proceed with creation of png files.
    $swf_files = file_scan_directory($files_directory, '/\.swf$/');
    $swf_number = count($swf_files);
    for ($i = 1; $i <= $swf_number; $i++) {
      $filename = $name_root . '_' . $i;
      if ( !_flexpaper2_exec_cmd(_swfrender_cmd($files_directory, $filename)) ) return false;
    }
  }
  return $success;
}

/**
 * Returns the pdf2swf conversion command.
 */
function _pdf2swf_cmd($pdf_path, $files_directory, $name_root, $start_page_number = 0, $end_page_number = 0) {
  if ($start_page_number > 0)
    return variable_get('flexpaper2_path_to_pdf2swf', 'pdf2swf') . ' "' . $pdf_path . '" -o "' . $files_directory . '/' . $name_root . '_%.swf" -p ' . $start_page_number . '-' . $end_page_number . ' -f -T 9 -t -s storeallcharacters';
  else
    return variable_get('flexpaper2_path_to_pdf2swf', 'pdf2swf') . ' "' . $pdf_path . '" -o "' . $files_directory . '/' . $name_root . '.swf" -f -T 9 -t -s storeallcharacters';
}

/**
 * Returns the swfrender conversion command.
 */
function _swfrender_cmd($files_directory, $filename) {
  return variable_get('flexpaper2_path_to_swfrender', 'swfrender') . ' "' . $files_directory . '/' . $filename . '.swf" -o "' . $files_directory . '/' . $filename . '.png" -X 1024 -s keepaspectratio';
}

/**
 * Returns the pdftk conversion command.
 */
function _pdftk_cmd($pdf_path, $files_directory, $name_root) {
  return variable_get('flexpaper2_path_to_pdftk', 'pdftk') . ' "' . $pdf_path . '" burst output "' . $files_directory . '/' . $name_root . '_%1d.pdf" compress';
}

/**
 * Returns the pdf2json conversion command.
 */
function _pdf2json_cmd($pdf_path, $files_directory, $fid) {
  return variable_get('flexpaper2_path_to_pdf2json', 'pdf2json') . ' "' . $pdf_path . '" -enc UTF-8 -compress -hidden "' . $files_directory . "/" . $fid . '.js"';
}

/**
 * Execute conversion command. Return FALSE if there was an error.
 */
function _flexpaper2_exec_cmd($command) {
  $output = shell_exec($command);
  return _flexpaper2_check_publishing_result($output);
}

/**
 * Check if there was an error in publishing. Return TRUE if error appeared.
 */
function _flexpaper2_check_publishing_result($output) {
  //@TODO this function is not tested with swfrender and pdf2json tools. We don't know how do error messages look like here.
  $pattern = '/(FATAL|ERROR)/i';
  preg_match($pattern, $output, $m);
  if (!empty($m)) {
    watchdog('flexpaper2', 'Error in pdf conversion process. Log info: %info', array('%info' => $output), WATCHDOG_ERROR);
    return false;
  }
  else {
    return true;
  }
}

/**
 * Helper function. Count number of pages in PDF file.
 */
function _flexpaper2_get_pdf_pages_number($filepath) {
  // This is a helper function. So, if something goes wrong here we can continue
  // without knowledge about the exact number of pages. That's why @ operator is
  // here.
  $stream = @fopen($filepath, "r");
  $pdf_content = @fread($stream, filesize($filepath));
  $first_value = 0;
  $second_value = 0;
  if (preg_match("/\/N\s+([0-9]+)/", $pdf_content, $matches)) {
    $first_value = $matches[1];
  }

  if (preg_match_all("/\/Count\s+([0-9]+)/s", $pdf_content, $matches)) {
    $second_value = max($matches[1]);
  }
  $max = (($second_value != 0) ? $second_value : max($first_value, $second_value));
  @fclose($stream);
  if ($max == 0) {
    if (extension_loaded('imagick')) {
      try {
        $im = new imagick($filepath);
        $max = $im->getNumberImages();
      }
      catch (Exception $ex) {
        watchdog('flexpaper2', 'Error in attempt to count number of pages using imagick. Exception: %info', array('%info' => $ex->getMessage()), WATCHDOG_ERROR);
      }
    }
  }
  return $max;
}
