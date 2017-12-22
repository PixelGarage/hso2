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
   * Full item click on call2action nodes.
   */
  Drupal.behaviors.fullItemClick = {
    attach: function () {
      var $call2actions = $('.node-call2action'),
          $news = $('.node-news.view-mode-teaser'),
          $pages = $('body.front .node-page.view-mode-teaser');

      // make call2action full clickable
      $call2actions.once('click', function() {
        $(this).on('click', function(ev) {
          window.location = $(this).find('.field-name-field-link a').attr('href');
          return false;
        });
      });
      $news.once('click', function() {
        $(this).on('click', function(ev) {
          window.location = $(this).find('>a').attr('href');
          return false;
        });
      });
      $pages.once('click', function() {
        $(this).on('click', function(ev) {
          window.location = $(this).find('>a').attr('href');
          return false;
        });
      });
    }
  };

  /**
   * Implements the active state of the page sub-header block menus and opens or closes the sub-header block section
   * according to the menu state.
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
   */

  /**
   * Scrolls smoothly to startdaten and info event tab due to menu click.
   */
  Drupal.behaviors.smoothScrolltoCourseTab = {
    attach: function(context, settings) {
      var $course = $('.node-course'),
        $courseBlockMenu = $course.find('.field-name-course-links'),
        $startDatenMenu = $courseBlockMenu.find('ul.menu>li.menu.startdaten'),
        $infoEventMenu = $courseBlockMenu.find('ul.menu>li.menu.infoevent'),
        $target = $('#views-bootstrap-tab-course-tabs');

      $infoEventMenu.once('clicked', function () {
        $(this).on('click', function () {
          var $infoTab = $target.find('.tab-info-events > a');

          // click tab
          $infoTab.click();

          // scroll to tab section
          if ($target.length > 0) {
            var topPos = $target.offset().top;
            $('html, body').animate({scrollTop: topPos-30}, 1000, 'swing');
            return false;
          }
          return true;
        });
      });
      $startDatenMenu.once('clicked', function () {
        $(this).on('click', function () {
          var $startdatenTab = $target.find('.tab-startdaten > a');

          // click tab
          $startdatenTab.click();

          // scroll to tab section
          if ($target.length > 0) {
            var topPos = $target.offset().top;
            $('html, body').animate({scrollTop: topPos-30}, 1000, 'swing');
            return false;
          }
          return true;
        });
      });

    }
  };

  /**
   * Enhance the links to Consulting- and Brochure-Form with a parameter holding the segment or course id
   * of the current webpage origin.
   */
  Drupal.behaviors.enhanceBlockMenuWithParameters = {
    attach: function() {
      var $course = $('.node-course'),
        $courseBlockMenu = $course.find('.field-name-course-links'),
        $menuLinks = $courseBlockMenu.find('ul.menu>li.menu.consulting').add('ul.menu>li.menu.brochures');

      //
      // Polyfill for IE11
      if (!String.prototype.startsWith) {
        String.prototype.startsWith = function(searchString, position){
          position = position || 0;
          return this.substr(position, searchString.length) === searchString;
        };
      }

      //
      // find segment taxonomy id or course node id
      var body_classes = $("body").attr("class").toString().split(' '),
        tid = false,
        nid = false;
      $.each(body_classes, function (i, className) {
        if (className.startsWith('page-node-')) {
          className = className.replace('page-node-', '');
          if (className.length > 0) {
            nid = className;
          }
        }
        else if (className.startsWith('page-taxonomy-term-')) {
          className = className.replace('page-taxonomy-term-', '');
          if (className.length > 0) {
            tid = className;
          }
        }
      });

      //
      // attach specific parameter to menu on click
      $menuLinks.once('clicked', function() {
        $(this).on('click', function() {
          var href = $(this).find('a').attr('href');

          if (nid) {
            window.location = href + '?nid=' + nid;
            return false;
          }
          else if (tid) {
            window.location = href + '?tid=' + tid;
            return false;
          }
        });
      });
    }
  };

  /**
   * Implements an interval per slide for the carousel.
   */
  Drupal.behaviors.carouselIntervalPerSlide = {
    attach: function() {
      var timeout,
        $carousel = $('#views-bootstrap-carousel-1'),
        interval = $carousel.find('.item.active').attr('data-interval'),
        start = interval > 5000 ? interval : 10000,
        _next_slide = function() {
          $carousel.carousel('next');
        };

      //initialize first slide
      timeout = setTimeout(function(){ $carousel.carousel({interval: 1000}); }, start-1000);

      // add events to carousel
      $carousel.once('carousel', function() {
        $carousel.on('slid.bs.carousel', function () {
          clearTimeout(timeout);
          var data = $(this).find('.item.active').attr('data-interval'),
            interval = data > 5000 ? data : 10000;

          timeout = setTimeout(_next_slide, interval);
        });

        $carousel.find('.carousel-control.left').on('click', function(){
          //
          clearTimeout(timeout);
        });

        $carousel.find('.carousel-control.right').on('click', function(){
          clearTimeout(timeout);
        });
      });

    }
  };

  /**
   * Consulting / Brochure webforms.
   *
   * Handles the dynamically shown select boxes in the brochure webform.
   */
  Drupal.behaviors.dynamicSelectBoxes = {
    attach: function () {
      var $webform = $('#webform-client-form-1335, #webform-client-form-1336, #webform-client-form-14006'),
        $compInteresse = $webform.find('.webform-component--interesse'),
        $selectInteresse = $webform.find('#edit-submitted-interesse'),
        $compAbschluss = $webform.find('.webform-component--abschluss'),
        $selectAbschluss = $webform.find('#edit-submitted-abschluss'),
        $compLehrgang = $webform.find('.webform-component--lehrgang'),
        $selectLehrgang = $webform.find('#edit-submitted-lehrgang'),
        _getURLParameter = function (sParamName) {
          var sPageURL = window.location.search.substring(1);
          var sURLParams = sPageURL.split('&');

          for (var i = 0; i < sURLParams.length; i++) {
            var sParam = sURLParams[i].split('=');
            if (sParam[0] === sParamName) {
              return sParam[1];
            }
          }
          return false;
        };

      // only for specific webforms
      if ($webform.length <= 0) return;

      //
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

      //
      // select correct option, if available in url
      var tid = _getURLParameter('tid'),
          nid = _getURLParameter('nid');

      if (tid) {
        // called from segment page, pre-fill taxonomy (interesse) select box
        $selectInteresse.val(tid).trigger('change');
      }
      else if (nid) {
        // called from course page, set only course select box, hide others
        $compInteresse.hide();
        $selectInteresse.val(1); // KVCollege
        $compAbschluss.hide();
        $selectAbschluss.val(63); // Lehrgänge
        $selectLehrgang.html('<option>Lehrgänge werden geladen...</option>');
        $compLehrgang.show(300);
        // load course
        $selectLehrgang.load(Drupal.settings.basePath + 'get_course/' + nid, function (response, status, xhr) {
          if (status == "error") {
            $selectLehrgang.html("Keine Daten gefunden");
          }
        });
      }
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
   * TELC webform
   *
   * Handles the kind of exam and calculates the price depending of the selected exam (oral, written, both).
   */
  Drupal.behaviors.telc_anmeldung = {
    attach: function (context) {
      var $telc_form = $('#webform-client-form-78061'),
        $radios_level = $telc_form.find('.webform-component--zertifikat-b--level-b input[type="radio"]')
        .add('.webform-component--zertifikat-a--level-a input[type="radio"]'),
        $radios_type = $telc_form.find('.webform-component--zertifikat-b--pruefungsart input[type="radio"]'),
        checkCount = function () {
          var $level = $telc_form.find('.webform-component--zertifikat-b--level-b input[type="radio"]:checked')
            .add('.webform-component--zertifikat-a--level-a input[type="radio"]:checked'),
            $type = $telc_form.find('.webform-component--zertifikat-b--pruefungsart input[type="radio"]:checked'),
            price =  0;

          // find price for checked radio, if any
          if ($level.length > 0) {
            var costs = Drupal.settings.hso_anmeldung.telc_costs,
              level = $level.attr('value');

            if (level === 'a1' || level === 'a2') {
              price = costs[level];
            } else if ($type.length > 0) {
              var type = $type.attr('value');

              price = costs[type + '_' + level];
            }
          }

          // set corresponding price in agb label
          $telc_form.find('.webform-component--agbcheck label strong.telc_costs').html('CHF ' + price + '.--');
        };

      // add click events to radios
      $radios_level.once('radio-level-click', function() {
        $(this).on('click', checkCount);
      });
      $radios_type.once('radio-type-click', function() {
        $(this).on('click', checkCount);
      });

      // initialize price
      checkCount();
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
