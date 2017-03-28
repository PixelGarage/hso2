<div id="views-bootstrap-tab-course-tabs" class="<?php print $classes ?>">
  <ul class="nav nav-<?php print $tab_type?> <?php if ($justified) print 'nav-justified' ?>">
    <li class="tab-startdaten active">
      <a href="#tab-startdaten" data-toggle="tab">Startdaten</a>
    </li>
    <li class="tab-info-events">
      <a href="#tab-info-events" data-toggle="tab">Informationsanlässe</a>
    </li>
    <?php foreach ($tabs as $key => $tab): ?>
     <li>
       <a href="#tab-<?php print "{$id}-{$key}" ?>" data-toggle="tab"><?php print $tab ?></a>
     </li>
    <?php endforeach ?>
  </ul>
  <div class="tab-content">
    <div class="tab-pane active" id="tab-startdaten">
      <?php print views_embed_view('course_times'); ?>
    </div>
    <div class="tab-pane" id="tab-info-events">
      <?php print views_embed_view('course_times', 'block_info_events'); ?>
    </div>
    <?php foreach ($rows as $key => $row): ?>
      <div class="tab-pane" id="tab-<?php print "{$id}-{$key}" ?>">
        <?php print $row ?>
      </div>
    <?php endforeach ?>
  </div>
</div>

