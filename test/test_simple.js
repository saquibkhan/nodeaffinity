(function() {
  'use strict';

  var nc = require('../');
  var assert = require('assert');

  var affinity = nc.getAffinity();
  var mask = 0;
  if (affinity > 1) {
    mask = 1;
  } else {
    mask = 2;
  }
  var newAffinity = nc.setAffinity(mask);
  assert.equal(newAffinity, mask);
  assert.equal(nc.getAffinity(), mask);

}());