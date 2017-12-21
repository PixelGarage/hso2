<div id="views-bootstrap-accordion-<?php print $id ?>" class="<?php print $classes ?>">
  <?php $index = 0; ?>
  <?php foreach ($course_categories as $category => $subcategories): ?>
    <div class="course-block">
      <div class="course-category">
        <h2><?php print $category ?></h2>
      </div>
      <?php foreach ($subcategories as $subcategory => $weights): ?>
        <?php if ($subcategory != '*'): ?>
          <?php $index++; ?>
          <div class="panel panel-default">
            <div class="panel-heading">
              <div class="panel-title">
                <a class="accordion-toggle collapsed"
                   data-toggle="collapse"
                   href="#collapse-<?php print $id . '-' . $index ?>">
                  <?php print $subcategory ?>
                </a>
              </div>
            </div>

            <div id="collapse-<?php print $id . '-' . $index ?>" class="panel-collapse collapse">
              <div class="panel-body">
                <?php foreach ($weights as $weight => $key): ?>
                  <div class="course-title">
                    <?php print $titles[$key] ?>
                  </div>
                <?php endforeach ?>
              </div>
            </div>
          </div>
        <?php else: ?>
          <?php foreach ($weights as $weight => $key): ?>
            <div class="course-title">
              <?php print $titles[$key] ?>
            </div>
          <?php endforeach ?>
        <?php endif; ?>
      <?php endforeach ?>
    </div>
  <?php endforeach ?>
</div>
