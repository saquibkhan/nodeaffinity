'use strict';

const assert = require('assert');

const nc = require('../');

const affinity = nc.getAffinity();

const mask = affinity > 1 ? 2 : 1;

const newAffinity = nc.setAffinity(mask);

assert.equal(newAffinity, mask, 'setAffinity failed to match');

assert.equal(nc.getAffinity(), mask, 'getAffinity failed to match');
