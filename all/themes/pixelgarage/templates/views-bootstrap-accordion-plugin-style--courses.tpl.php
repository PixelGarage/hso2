<div id="views-bootstrap-accordion-<?php print $id ?>" class="<?php print $classes ?>">
  <?php foreach ($rows as $key => $row): ?>
    <?php if (!empty($course_subject_titles[$key])): ?>
      <div class="main-title">
        <h1><?php print $course_subject_titles[$key] ?></h1>
      </div>
    <?php endif; ?>
    <?php if (!empty($course_subject_description[$key])): ?>
      <div class="main-description">
        <?php print $course_subject_description[$key] ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($course_subjects[$key])): ?>
      <div class="course-subject">
        <h2><?php print $course_subjects[$key] ?></h2>
      </div>
    <?php endif; ?>

    <div class="panel panel-default">
      <div class="panel-heading">
        <h4 class="panel-title">
          <?php print $titles[$key] ?>
        </h4>
        <span class="button short-description">
          <a class="accordion-toggle"
              data-toggle="collapse"
              href="#collapse<?php print $key ?>">
             <?php print $label_short_desc ?>
          </a>
          <span class="fa fa-angle-down"></span>
        </span>
      </div>

      <div id="collapse<?php print $key ?>" class="panel-collapse collapse">
        <div class="panel-body">
          <?php print $row ?>
        </div>
      </div>
    </div>
  <?php endforeach ?>
</div>
