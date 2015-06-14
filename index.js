(function () {
  'use strict';
  var nc = require('./build/Release/nodeaffinity');

  module.exports =  {
    'getAffinity': nc.getAffinity,
    'setAffinity': nc.setAffinity
  };

}());

