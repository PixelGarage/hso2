(function ($) {

  /**
   * Initializes all flexpaper viewers.
   */
  Drupal.behaviors.flexpaper2Viewer = {
    attach: function (context, settings) {
      // initialize Flexpaper viewer
      var flexpaper2_init = function (fieldName, index, settings) {
        var fileSettings  = settings[index],
            fieldClass    = '.field-name-' + fieldName,
            $linkCont     = $(fieldClass + ' .flexpaper-link-container'),
            $container    = $(fieldClass + ' .flexpaper_viewer_container'),
            element_id    = 'flexpaper-viewer-' + fieldName;

        // reset flexpaper element structure
        $container.empty();
        $container.removeClass('flexpaper_viewer_container').addClass('flexpaper_viewer').removeAttr('style');

        // we need an id on the viewer for flexpaper
        $(fieldClass + ' .flexpaper_viewer').attr('id', element_id);

        // add content to flexpaper container
        if (fileSettings.show_flexpaper) {
          // display flexpaper viewer
          $('#' + element_id).FlexPaperViewer(
            {
              config: {
                //SWFFile: fileSettings.swfFiles,
                JSONFile: fileSettings.jsonFiles,
                IMGFiles: fileSettings.imgFiles,
                PDFFile: fileSettings.pdfFiles,
                ThumbIMGFiles: fileSettings.thumbFiles,

                Scale: parseFloat(fileSettings.Scale),
                ZoomTransition: fileSettings.ZoomTransition,
                ZoomTime: parseFloat(fileSettings.ZoomTime),
                ZoomInterval: parseFloat(fileSettings.ZoomInterval),
                FitPageOnLoad: fileSettings.FitPageOnLoad == 1,
                FitWidthOnLoad: fileSettings.FitWidthOnLoad == 1,
                FullScreenAsMaxWindow: fileSettings.FullScreenAsMaxWindow == 1,
                ProgressiveLoading: fileSettings.ProgressiveLoading == 1,
                MinZoomSize: parseFloat(fileSettings.MinZoomSize),
                MaxZoomSize: parseFloat(fileSettings.MaxZoomSize),
                SearchMatchAll: fileSettings.SearchMatchAll == 1,
                InitViewMode: fileSettings.InitViewMode,
                RenderingOrder: fileSettings.RenderingOrder,

                ViewModeToolsVisible: fileSettings.ViewModeToolsVisible == 1,
                ZoomToolsVisible: fileSettings.ZoomToolsVisible == 1,
                NavToolsVisible: fileSettings.NavToolsVisible == 1,
                CursorToolsVisible: fileSettings.CursorToolsVisible == 1,
                SearchToolsVisible: fileSettings.SearchToolsVisible == 1,
                UIConfig: fileSettings.UIConfig,

                jsDirectory: settings.jsDirectory,
                cssDirectory: settings.cssDirectory,
                localeDirectory: settings.localeDirectory,

                StartAtPage : 1,
                MixedMode : true,
                EnableWebGL : true,
                AutoAdjustPrintSize : true,
                PrintPaperAsBitmap : false,
                AutoDetectLinks :true,
                PublicationTitle : '',

                localeChain: 'en_US',
                WMode : 'transparent',
                //JSONDataType: 'json',
                key: settings.licenseKey
              }
            }
          );

        } else {
          // display flexpaper info message
          $('#' + element_id).html(fileSettings.flexpaper_info);

        }

        // add file link if any
        if (fileSettings.hasOwnProperty('file_link')) {
          $linkCont.html(fileSettings.file_link);
        }

      };

      // iterate over all file fields with flexpaper formatter enabled
      $.each(Drupal.settings.flexpaper2, function (fieldName, settings) {
        var fieldClass  = '.field-name-' + fieldName,
            $selectItem = $(fieldClass + ' .flexpaper-select');

        // add select change event
        $selectItem.once('select', function() {
          $(this).on('change', function() {
            var value = $(this).find('option:selected').attr('value');

            flexpaper2_init(fieldName, value, settings);
          });
        });

        // select first file in select box
        var val = $selectItem.find('option:first-child').val();
        $selectItem.change();
      });

    }
  }
})(jQuery);
