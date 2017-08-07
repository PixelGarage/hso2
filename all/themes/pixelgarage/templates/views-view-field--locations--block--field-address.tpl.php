<?php $address = $row->field_field_address[0]['raw']; ?>
<div class="organisation-block"><strong><?php print check_plain($address['organisation_name']); ?></strong></div>
<div class="street-block"><?php print check_plain($address['thoroughfare']); ?></div>
<div class="locality-block"><?php print check_plain($address['country']); ?>-<?php print check_plain($address['postal_code']); ?> <?php print check_plain($address['locality']); ?></div>
<div class="phone-number"><span class="icon-phone"></span> <?php print check_plain($address['phone_number']); ?></div>
