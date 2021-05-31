const rea = require('reajs')

require('smartmenus')
require('smartmenus/dist/css/sm-core-css.css')
require('smartmenus/dist/css/sm-simple/sm-simple.css')

$('#main-menu').smartmenus({
    mainMenuSubOffsetX: -1,
    subMenusSubOffsetX: 10,
    subMenusSubOffsetY: 0
})

$('#main-menu').on('click.smapi', function(e, item) {
    // check namespace if you need to differentiate from a regular DOM event fired inside the menu tree
    if (e.namespace == 'smapi') {
        let cmd = item.textContent.replace(" ", "_")
        rea.pipelines().run(cmd, "")
    // your handler code
    }
})