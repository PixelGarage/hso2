<div id="views-bootstrap-tab-<?php print $id ?>" class="<?php print $classes ?>">
  <ul class="nav nav-<?php print $tab_type?> <?php if ($justified) print 'nav-justified' ?>">
    <?php foreach ($tabs as $key => $tab): ?>
     <li class="<?php if ($key === 0) print 'active'?>">
       <a href="#tab-<?php print "{$id}-{$key}" ?>" data-toggle="tab"><?php print $tab ?></a>
     </li>
    <?php endforeach ?>
  </ul>
  <div class="tab-content">
    <?php foreach ($locations as $key => $location): ?>
      <div class="tab-pane <?php if ($key === 0) print 'active'?>" id="tab-<?php print "{$id}-{$key}" ?>">
        <div class="row">
          <div class="col-sm-6">
            <div class="organisation-block"><strong><?php print $location['organisation_name']; ?></strong></div>
            <div class="street-block"><?php print $location['thoroughfare']; ?></div>
            <div class="locality-block"><?php print $location['country']; ?>-<?php print $location['postal_code']; ?> <?php print $location['locality']; ?></div>
            <div class="phone-number"><span class="icon-phone"></span> <?php print $location['phone_number']; ?></div>
            <div class="email"><span class="icon-mail"></span> <?php print $location['email']; ?></div>
          </div>
          <div class="col-sm-6">
            <!-- print location map -->
            <div class="map-location">
              <a href="https://maps.google.com/maps?q=<?php print $location['map_location']; ?>" target="_blank">
                <img src="https://maps.googleapis.com/maps/api/staticmap?center=<?php print $location['map_location']; ?>&language=de&size=250x173&maptype=roadmap&markers=color:red%7C<?php print $location['map_location']; ?>&sensor=false" alt="" />
              </a>
            </div>
          </div>
        </div>
      </div>
    <?php endforeach ?>
  </div>
</div>

