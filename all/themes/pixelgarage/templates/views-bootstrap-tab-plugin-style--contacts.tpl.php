<div id="views-bootstrap-tab-<?php print $id ?>" class="<?php print $classes ?>">
  <ul class="nav nav-<?php print $tab_type?> <?php if ($justified) print 'nav-justified' ?>">
    <?php $index = 0 ?>
    <?php foreach ($tabs as $location => $tab): ?>
      <li class="<?php if ($index === 0) print 'active'?>">
        <a href="#tab-<?php print "{$id}-{$index}" ?>" data-toggle="tab"><?php print $location ?></a>
      </li>
      <?php $index++ ?>
    <?php endforeach ?>
  </ul>
  <div class="tab-content">
    <?php $index = 0 ?>
    <?php foreach ($tabs as $location => $row_indexes): ?>
      <div class="tab-pane <?php if ($index === 0) print 'active'?>" id="tab-<?php print "{$id}-{$index}" ?>">
        <?php foreach ($row_indexes as $row_index): ?>
          <div class="tab-pane-item contact">
            <?php print $rows[$row_index] ?>
          </div>
        <?php endforeach ?>
      </div>
      <?php $index++ ?>
    <?php endforeach ?>
  </div>
</div>

