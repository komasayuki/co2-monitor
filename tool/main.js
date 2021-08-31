const minify = require('minify');

(async function(){
    const minifiedData = await minify(__dirname + '/monitor.html', {
    });

    const result = '#define INDEX_HTML (F("' + minifiedData.replace(/"/g, '\\"') + '"))';
    console.log(result);
    require('fs').writeFileSync(__dirname + '/../src/indexhtml.h', result);
})();

