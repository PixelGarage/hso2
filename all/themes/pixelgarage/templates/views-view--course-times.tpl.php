<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
?>
<div class="<?php print $classes; ?>">
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <?php print $title; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>
  <?php if ($rows): ?>
    <div class="view-content">
      <ul class="nav nav-tabs">
        <?php $index = 0; ?>
        <?php foreach ($tabs as $tab_title => $tab_content): ?>
          <li class="tab-table-<?php print $index ?> <?php if ($index == 0) print 'active' ?>">
            <a href="#tab-table-<?php print $display_id . $index++ ?>" data-toggle="tab"><?php print $tab_title ?></a>
          </li>
        <?php endforeach ?>
      </ul>
      <div class="tab-content">
        <?php $index = 0; ?>
        <?php foreach ($tabs as $tab_title => $tab_content): ?>
          <div class="tab-pane <?php if ($index == 0) print 'active' ?>" id="tab-table-<?php print $display_id . $index++ ?>">
            <!-- Print grouped tables as accordion -->
            <div id="views-bootstrap-accordion-<?php print $index ?>" class="panel-group date-accordion" aria-multiselectable="true">
              <?php $key = 0; ?>
              <?php foreach ($tab_content as $date_title => $table): ?>
                <div class="panel panel-default">
                  <div class="panel-heading">
                    <div class="panel-title">
                      <a class="accordion-toggle"
                         data-toggle="collapse"
                         aria-expanded="true"
                         href="#collapse-<?php print $index ?>-<?php print $key ?>">
                        <?php print $date_title ?>
                      </a>
                    </div>
                  </div>

                  <div id="collapse-<?php print $index ?>-<?php print $key ?>" class="panel-collapse collapse <?php if (0 == $key++) print 'in' ?>">
                    <div class="panel-body">
                      <?php print $table; ?>
                    </div>
                  </div>
                </div>
              <?php endforeach ?>
            </div>
          </div>
        <?php endforeach ?>
      </div>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div>
