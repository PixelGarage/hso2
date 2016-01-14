/**
 * This file contains all Drupal behaviours of the Apia theme.
 *
 * Created by ralph on 05.01.14.
 */

(function ($) {

  /**
   * Brochure webform.
   *
   * Handles the dynamically shown select boxes in the brochure webform.
   */
  Drupal.behaviors.dynamicSelectBoxes = {
    attach: function () {
      var $webform         = $('form#webform-client-form-1336'),
          $selectInteresse = $webform.find('#edit-submitted-interesse'),
          $compAbschluss   = $webform.find('.webform-component--abschluss'),
          $selectAbschluss = $webform.find('#edit-submitted-abschluss'),
          $compLehrgang    = $webform.find('.webform-component--lehrgang'),
          $selectLehrgang  = $webform.find('#edit-submitted-lehrgang');

      // hide select boxes and reset interesse select box
      $compAbschluss.hide();
      $compLehrgang.hide();
      $selectInteresse.val(-1);

      $selectInteresse.once('change', function () {
        $(this).change(function () {
          var tid = $selectInteresse.find('option:selected').val();

          $selectAbschluss.html('<option>Auswahl wird geladen...</option>');
          $compAbschluss.show(300);
          $compLehrgang.hide();
          $selectAbschluss.load(Drupal.settings.basePath + 'get_graduations/' + tid, function (response, status, xhr) {
            if (status == "error") {
              $selectAbschluss.html("Keine Daten gefunden");
            }
          });

        });
      });
      $selectAbschluss.once('change', function () {
        $(this).change(function () {
          var tid  = $selectInteresse.find('option:selected').val(),
              tid2 = $selectAbschluss.find('option:selected').val();

          $selectLehrgang.html('<option>Lehrgänge werden geladen...</option>');
          $compLehrgang.show(300);
          $selectLehrgang.load(Drupal.settings.basePath + 'get_courses/' + tid + '/' + tid2, function (response, status, xhr) {
            if (status == "error") {
              $selectLehrgang.html("Keine Daten gefunden");
            }
          });

        });
      });
    }
  };

  /**
   * ECDL webform
   *
   * Handles the modules of the ECDL-Anmeldung and calculates the price depending of the selected modules.
   */
  Drupal.behaviors.ecdl_anmeldung = {
    attach: function (context) {
      $('#webform-client-form-476', context).once(function () {
        var checkCount = function () {
          var _f            = $('#webform-client-form-476');
          var total_modules = _f.find('#webform-component-module input:checkbox:checked').add('#webform-component-module-base input:checkbox:checked').size();
          if (total_modules > 2) {
            alert("Information\n_______________________________________________________\n\nPro Pr\u00FCfungstermin k\u00F6nnen Sie maximal zwei Module absolvieren.\n\nBitte melden Sie sich f\u00FCr weitere Module an einem\nanderen Tag an!");
            return false;
          }
          _f.find('#webform-component-agbcheck label strong').html('CHF ' + (total_modules * 65) + '.--');
        };
        $(this).find('#webform-component-module input:checkbox').add('#webform-component-module-base input:checkbox').click(checkCount);
        checkCount();
      });
    }
  };

  /**
   * This behavior adds shadow to header on scroll.
   *
  Drupal.behaviors.addHeaderShadow = {
    attach: function (context) {
      $(window).on("scroll", function () {
        if ($(window).scrollTop() > 10) {
          $("header.navbar .container").css("box-shadow", "0 4px 3px -4px gray");
        }
        else {
          $("header.navbar .container").css("box-shadow", "none");
        }
      });
    }
  };
   */

  /**
   * Allows full size clickable items.
   Drupal.behaviors.fullSizeClickableItems = {
    attach: function () {
      var $clickableItems = $('.node-link-item.node-teaser .field-group-div')
        .add('.node-team-member.node-teaser .field-group-div');

      $clickableItems.once('click', function () {
        $(this).on('click', function () {
          window.location = $(this).find("a:first").attr("href");
          return false;
        });
      });
    }
  };
   */

  /**
   * Swaps images from black/white to colored on mouse hover.
   Drupal.behaviors.hoverImageSwap = {
    attach: function () {
      $('.node-project.node-teaser .field-name-field-images a img').hover(
        function () {
          // mouse enter
          src = $(this).attr('src');
          $(this).attr('src', src.replace('teaser_bw', 'teaser_normal'));
        },
        function () {
          // mouse leave
          src = $(this).attr('src');
          $(this).attr('src', src.replace('teaser_normal', 'teaser_bw'));
        }
      );
    }
  }
   */

  /**
   * Open file links in its own tab. The file field doesn't implement this behaviour right away.
   Drupal.behaviors.openDocumentsInTab = {
    attach: function () {
      $(".field-name-field-documents").find(".field-item a").attr('target', '_blank');
    }
  }
   */

})(jQuery);
