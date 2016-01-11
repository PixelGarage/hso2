<div id="views-bootstrap-tab-<?php print $id ?>" class="<?php print $classes ?>">
  <ul class="nav nav-<?php print $tab_type?> <?php if ($justified) print 'nav-justified' ?>">
    <li class="active">
      <a href="#tab-startdaten" data-toggle="tab">Startdaten</a>
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
    <?php foreach ($rows as $key => $row): ?>
      <div class="tab-pane" id="tab-<?php print "{$id}-{$key}" ?>">
        <?php print $row ?>
      </div>
    <?php endforeach ?>
  </div>
</div>

