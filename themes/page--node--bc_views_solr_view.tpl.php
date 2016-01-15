<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>

<div id="page">

  <header class="header" id="header" role="banner">

    <?php if ($logo): ?>
      <a href="http://barnard.edu" title="Barnard College" class="header__logo" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="header__logo-image" /></a>
    <?php endif; ?>

    <?php if ($secondary_menu && !empty($secondary_menu_heading)): ?>
      <nav class="header__secondary-menu" id="secondary-menu" role="navigation">
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => $secondary_menu_heading,
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </nav>
    <?php endif; ?>

    <?php print render($page['header']); ?>
  </header>

  <div id="navigation">

    <?php if ($site_name || $site_slogan): ?>
      <div class="header__name-and-slogan" id="name-and-slogan">
        <?php if ($site_name): ?>
          <h1 class="header__site-name" id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="header__site-link" rel="home"><span><?php print $site_name; ?></span></a>
          </h1>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div class="header__site-slogan" id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php if ($main_menu): ?>
      <nav id="main-menu" role="navigation" tabindex="-1">
        <?php
        // This code snippet is hard to modify. We recommend turning off the
        // "Main menu" on your sub-theme's settings form, deleting this PHP
        // code block, and, instead, using the "Menu block" module.
        // @see https://drupal.org/project/menu_block
        print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </nav>
    <?php endif; ?>

    <?php print render($page['navigation']); ?>

  </div>
      
  <div id="main">
    <div id="content" class="column" role="main">
      <?php print render($page['highlighted']); ?>
      <?php print render($title_prefix); ?>
      <?php if (isset($bc_breadcrumb)): ?>
        <?php print $bc_breadcrumb; ?>
      <?php endif; ?>
      <?php
        $bc_views_breadcrumb = array('breadcrumb'=>menu_get_active_breadcrumb());
        print theme('bc_islandora_breadcrumb', $bc_views_breadcrumb);
        isset($breadcrumb) ? print $breadcrumb : NULL;
        ?> 
      <?php if ($title): ?>
        <h1 class="page__title title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <div class="results"><span class="results-for"><?php print t('Results for'); ?></span><?php print $breadcrumb; ?></div>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
      <?php if (isset($socialmedia)): ?>
        <span class="socialmedia">
          <?php print $socialmedia; ?>
        </span>
      <?php endif; ?>
      <?php // TODO ?>
      <?php //if (isset($permalink)): ?>
        <?php //print $permalink; ?>
      <?php //endif;?>
    </div>

    <?php
      // Render the sidebars to see if there's anything in them.
      $sidebar_first  = render($page['sidebar_first']);
      $sidebar_second = render($page['sidebar_second']);
    ?>

    <?php if ($sidebar_first || $sidebar_second): ?>
      <aside class="sidebars">
        <?php print $sidebar_first; ?>
        <?php print $sidebar_second; ?>
      </aside>
    <?php endif; ?>

  </div>

  <?php print render($page['footer']); ?>

</div>

<?php print render($page['bottom']); ?>
