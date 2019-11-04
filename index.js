(function () {
  'use strict';
  var nc = require('bindings')('nodeaffinity');

  module.exports =  {
    'getAffinity': nc.getAffinity,
    'setAffinity': nc.setAffinity
  };

}());

