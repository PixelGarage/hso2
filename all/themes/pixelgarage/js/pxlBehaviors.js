/**
 * This file contains all Drupal behaviours of the Apia theme.
 *
 * Created by ralph on 05.01.14.
 */

(function ($) {

  Drupal.behaviors.activateMasonryFilters = {
    attach: function () {
      var $exposedForm = $('#views-exposed-form-social-masonry-panel-pane-1'),
        $filters = $exposedForm.find('.form-radios .control-label'),
        $selectedRadio = $exposedForm.find('input[checked=checked]');

      // add active class for selected radio
      $filters.removeClass('active');
      $selectedRadio.parent().addClass('active');
    }
  };

  /**
   * Implements the active state of the page sub-header block menus and opens or closes the sub-header block section
   * according to the menu state.
   */
  Drupal.behaviors.activateBlockMenus = {
    attach: function () {
      var $blockPanel = $('header#page-header .panel-header-blocks'),
        $blocks = $blockPanel.find('.region-header-blocks > .block'),
        $segmentBlockMenu = $('#block-menu-menu-segment-blocks'),
        $segmentMenus = $segmentBlockMenu.find('ul.menu>li.menu-block'),
        $courseBlockMenu = $('#block-menu-menu-course-blocks'),
        $courseMenus = $courseBlockMenu.find('ul.menu>li.menu-block');

      $segmentMenus.once('activated', function () {
        // add click events to all menus
        $(this).on('click', function () {
          var $menu = $(this),
            blockClass = '.' + $menu.attr('id'),
            $menuBlock = $blockPanel.find(blockClass), // menu id is block class
            $menuIsActive = $menu.hasClass('active');

          // deactivate all menus and hide all blocks in panel
          $segmentMenus.removeClass('active');
          $blocks.hide();

          // show / hide filter section
          if ($menuIsActive) {
            // deactivate menu and hide block panel
            $menu.removeClass('active');
            $blockPanel.slideUp(400);

          } else {
            // show menu block
            $menu.addClass('active');
            $menuBlock.show();

            // show panel content
            $blockPanel.slideDown(400);
          }
        });

        // activate first menu immediately
        if ($(this).hasClass('first')) {
          $(this).click();
        }

      });

      $courseMenus.once('activated', function () {
        // add click events to all menus
        $(this).on('click', function () {
          var $menu = $(this),
            blockClass = '.' + $menu.attr('id'),
            $menuBlock = $blockPanel.find(blockClass), // menu id is block class
            $menuIsActive = $menu.hasClass('active');

          // deactivate all menus and hide all blocks in panel
          $courseMenus.removeClass('active');
          $blocks.hide();

          // show / hide filter section
          if ($menuIsActive) {
            // deactivate menu and hide block panel
            $menu.removeClass('active');
            $blockPanel.slideUp(400);

          } else {
            // show menu block
            $menu.addClass('active');
            $menuBlock.show();

            // show panel content
            $blockPanel.slideDown(400);
          }
        });

        // activate first menu immediately
        if ($(this).hasClass('first')) {
          $(this).click();
        }

      });

    }
  };

  /**
   * Brochure webform.
   *
   * Handles the dynamically shown select boxes in the brochure webform.
   */
  Drupal.behaviors.dynamicSelectBoxes = {
    attach: function () {
      var $webform = $('#webform-client-form-1336, #webform-client-form-14006'),
        $selectInteresse = $webform.find('#edit-submitted-interesse'),
        $compAbschluss = $webform.find('.webform-component--abschluss'),
        $selectAbschluss = $webform.find('#edit-submitted-abschluss'),
        $compLehrgang = $webform.find('.webform-component--lehrgang'),
        $selectLehrgang = $webform.find('#edit-submitted-lehrgang');

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
          var tid = $selectInteresse.find('option:selected').val(),
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
          var _f = $('#webform-client-form-476');
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
   * Implements google analytic events for the brochure download and the consulting request forms.
   */
  Drupal.behaviors.gaEvents = {
    attach: function () {
      //
      // GA Events for the brochure download, consulting requests and brochure postal delivery
      var $brochureForm = $('#webform-client-form-1336'),
        $brochurePostal = $('#webform-client-form-14006'),
        $consultingForm = $('#webform-client-form-1335'),
        _createFunctionWithTimeout = function (callback, opt_timeout) {
          var called = false;
          var fn = function () {
            if (!called) {
              called = true;
              callback();
            }
          };
          setTimeout(fn, opt_timeout || 1000);
          return fn;
        };

      //
      // Adds a listener for the brochure download.
      $brochureForm.off('submit');
      $brochureForm.on('submit', function (event) {
        // Prevents the browser from submitting the form
        // and thus unloading the current page.
        event.preventDefault();

        // get form values
        var segment = $(this).find('.webform-component--interesse select option:selected').html(),
          course = $(this).find('.webform-component--lehrgang select option:selected').html(),
          label = segment + ' - ' + course;

        // Sends the event to Google Analytics and
        // resubmits the form once the hit is done.
        ga('send', 'event', 'Brochure-download-form', 'submit', label, {
          hitCallback: _createFunctionWithTimeout(function () {
            $brochureForm.off('submit');  // prevents infinite loop
            $brochureForm.submit();
          })
        });
      });

      //
      // Adds a listener for the brochure postal delivery.
      $brochurePostal.off('submit');
      $brochurePostal.on('submit', function (event) {
        // Prevents the browser from submitting the form
        // and thus unloading the current page.
        event.preventDefault();

        // get form values
        var segment = $(this).find('.webform-component--interesse select option:selected').html(),
          course = $(this).find('.webform-component--lehrgang select option:selected').html(),
          stao = $(this).find('.webform-component--standort select option:selected').html(),
          label =  stao + ' - ' + segment + ' - ' + course;

        // Sends the event to Google Analytics and
        // resubmits the form once the hit is done.
        ga('send', 'event', 'Brochure-delivery-form', 'submit', label, {
          hitCallback: _createFunctionWithTimeout(function () {
            $brochurePostal.off('submit');  // prevents infinite loop
            $brochurePostal.submit();
          })
        });
      });

      //
      // Adds a listener for the consulting form.
      $consultingForm.off('submit');
      $consultingForm.on('submit', function (event) {
        // Prevents the browser from submitting the form
        // and thus unloading the current page.
        event.preventDefault();

        // get form values
        var segment = $(this).find('.webform-component--interesse select option:selected').html(),
          stao = $(this).find('.webform-component--standort select option:selected').html(),
          label = segment + ' - ' + stao;

        // Sends the event to Google Analytics and
        // resubmits the form once the hit is done.
        ga('send', 'event', 'Consulting-form', 'submit', label, {
          hitCallback: _createFunctionWithTimeout(function () {
            $consultingForm.off('submit'); // prevents infinite loop
            $consultingForm.submit();
          })
        });
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
