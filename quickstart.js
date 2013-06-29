var uuid = require('./uuid.js');

console.log('unparse(generate()): ' + uuid.unparse(uuid.generate()));

console.log('unparse(generate_random()): ' + uuid.unparse(uuid.generate_random()));

console.log('unparse(generate_time()): ' + uuid.unparse(uuid.generate_time()));

var aUuid = uuid.generate();
uuid.clear(aUuid);
console.log('clear: ' + uuid.unparse(aUuid));

var aUuid1 = uuid.generate();
var aUuid2 = uuid.generate();
console.log('compare: ' + uuid.compare(aUuid1, aUuid2));

var aUuid1Copy = uuid.copy(aUuid1);
console.log('compare copy: ' + uuid.compare(aUuid1, aUuid1Copy));

console.log('is_null true: ' + uuid.is_null(aUuid));
console.log('is_null false: ' + uuid.is_null(aUuid1));

var aUuidTime = uuid.generate_time();
setTimeout(function() {
  console.log('time 2s: ' + (+new Date() - uuid.time(aUuidTime)) + 'ms');
}, 2000);

var aUuid1Str = uuid.unparse(aUuid1);
console.log('compare unparse parse: ' + uuid.compare(aUuid1, uuid.parse(aUuid1Str)));

console.log('unparse_lower(generate()): ' + uuid.unparse_lower(uuid.generate()));

console.log('unparse_upper(generate()): ' + uuid.unparse_upper(uuid.generate()));

console.log('generate_string_lower(): ' + uuid.generate_string_lower());

console.log('generate_string_upper(): ' + uuid.generate_string_upper());
