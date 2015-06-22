var generateProject = require('diy').generateProject
var uid = require('uid')

var opencv_lopts = '$(shell pkg-config --libs opencv)'
var opencv_copts = '$(shell pkg-config --cflags opencv)'

var copts = `\\
    -std=c++11 \\
    -I. \\
    -Ilib3rd \\
    -Ilib3rd/docopt \\
    -Ilib3rd/CF++/include \\
    ${opencv_copts}`

var lopts = `\\
    -framework AVFoundation \\
    -framework CoreServices \\
    -framework CoreFoundation \\
    -framework CoreGraphics \\
    -framework ImageIO \\
    ${opencv_lopts}`

generateProject(function (_) {
  "use strict"
  _.clang = (src, ...deps) => {
    var command = (_) => `clang++ ${copts} -c ${_.source} -o ${_.product}`
    var product = (_) => `${_.source.replace(/\..*/, '.o')}`
    _.compileFiles(...([command, product, src].concat(deps)))
  }

  _.clangExe = (body) => {
    var command = (_) => `clang++ ${lopts} ${_.sources.join(' ')} -o ${_.product}`
    var product = () => `clang-${uid(4)}.x`
    _.reduceFiles(command, product, body)
  }


  _.collectSeq("all", function (_) {
    _.toFile("bin/screen.x", (_) => {
      _.clangExe((_) => {
          _.clang("src/*.cpp", "src/*.hpp", "lib3rd/**/*.{h,hpp}");
          _.clang("lib3rd/CF++/source/*.cpp", "lib3rd/CF++/include/**/*.h");
          _.clang("lib3rd/docopt/docopt.cpp", "lib3rd/docopt/docopt.h");
          _.clang("lib3rd/json11/json11.cpp", "lib3rd/json11/json11.hpp");
		  _.clang("lib3rd/cppformat/*.cc", "lib3rd/cppformat/*.h");
      })
    })
    _.cmd("chmod +x bin/screen.x")
  })
})
