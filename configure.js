var generateProject = require('diy').generateProject
var uid = require('uid')

//var opencv_lopts = '$(shell pkg-config --libs opencv)'
//var opencv_copts = '$(shell pkg-config --cflags opencv)'

var opencv_lopts = "-L/usr/local/Cellar/opencv/2.4.11/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab"

var opencv_copts = "-I/usr/local/Cellar/opencv/2.4.11/include/opencv -I/usr/local/Cellar/opencv/2.4.11/include"

var copts = `\\
    -std=c++11 \\
    -I. \\
    -Ilib3rd \\
    -Ilib3rd/docopt \\
    -Ilib3rd/CF++/include \\
    -I/usr/local/include \\
    ${opencv_copts}`

var lopts = `\\
    -framework AVFoundation \\
    -framework CoreServices \\
    -framework CoreFoundation \\
    -framework CoreGraphics \\
    -framework ImageIO \\
    ${opencv_lopts} \\
    -L/usr/local/lib \\
    -lfolly \\
    -lglog`

generateProject(function (_) {
  "use strict"
  _.clang = (src, ...deps) => {
    var command = (_) => `clang++ -g ${copts} -c ${_.source} -o ${_.product}`
    var product = (_) => `${_.source.replace(/\..*/, '.o')}`
    _.compileFiles(...([command, product, src].concat(deps)))
  }

  _.clangExe = (body) => {
    var command = (_) => `clang++ -g ${lopts} ${_.sources.join(' ')} -o ${_.product}`
    var product = () => `clang-${uid(4)}.x`
    _.reduceFiles(command, product, body)
  }

	_.collect("docs", _ => {
		_.cmd("./node_modules/.bin/git-hist readme.md")
	});


  _.collectSeq("all", function (_) {
    _.toFile("bin/teasy.x", (_) => {
      _.clangExe((_) => {
          _.clang("src/*.cpp", "src/*.hpp", "lib3rd/**/*.{h,hpp}");
          _.clang("lib3rd/CF++/source/*.cpp", "lib3rd/CF++/include/**/*.h");
          _.clang("lib3rd/docopt/docopt.cpp", "lib3rd/docopt/docopt.h");
          _.clang("lib3rd/json11/json11.cpp", "lib3rd/json11/json11.hpp");
		  _.clang("lib3rd/cppformat/*.cc", "lib3rd/cppformat/*.h");
      })
    })
    _.cmd("chmod +x bin/teasy.x")
  })
})
