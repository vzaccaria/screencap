var generateProject = require('diy').generateProject
var uid = require('uid')

var copts = `-ICF++/include`
var lopts = `-framework CoreFoundation`

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

  _.collect("all", function (_) {
    _.toFile("screen.x", (_) => {
      _.clangExe((_) => {
        _.clang("screen.cpp")
        _.clang("CF++/source/*.cpp", "CF++/include/**/*.h")
      })
    })
  })

  // _.collect("clean", function (_) {
  //   _.cmd("rm -rf vhdl")
  //   _.cmd("rm -f *.hi *.o *.s *.dyn_hi *.dyn_o")
  // })
})