#!/usr/bin/env node
'use strict';

var fs = require('fs');
var path = require('path');

function make(name) {
	name = name.trim();

	var outHeader = name + '.h';
	var outSource = name + '.cc';
	var baseName = path.basename(name);
	var guardName = ('ARUAB_' + name.replace(/[^a-zA-Z0-9]+/g, '_') + '_H__').toUpperCase();
	var className = baseName.replace(/(?:^|\-+)([a-zA-Z0-9]+)/g, function (m, n) { return n[0].toUpperCase() + n.substring(1); });

	// header
	var headerLines = [
		'#ifndef ' + guardName,
		'#define ' + guardName,
		'#pragma once',
		'',
		'namespace arua {',
		'',
		'class ' + className + ' {',
		'public:',
		'\t' + className + '();',
		'\tvirtual ~' + className + '() = default;',
		'};',
		'',
		'}',
		'',
		'#endif',
		''
	];

	var header = headerLines.join('\n');
	fs.writeFileSync(outHeader, header, 'utf8');

	// source
	var sourceLines = [
		'#include "' + path.basename(outHeader) + '"',
		'',
		'using namespace arua;',
		'using namespace std;',
		'',
		className + '::' + className + '() {',
		'}',
		''
	];

	var source = sourceLines.join('\n');
	fs.writeFileSync(outSource, source, 'utf8');
}

for (var i = 2; i < process.argv.length; i++) {
	var name = process.argv[i].trim();
	if (fs.existsSync(name + '.h') || fs.existsSync(name + '.cc')) {
		throw new Error('Refusing to overwrite files for base name: ' + name);
	}

	make(name);
}
