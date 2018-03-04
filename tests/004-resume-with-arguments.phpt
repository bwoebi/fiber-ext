--TEST--
tests Fiber resume args
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
	echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber(function ($a) {
    var_dump($a);
    var_dump(Fiber::yield());
    var_dump(Fiber::yield());
    var_dump(Fiber::yield());
});
$f->start("string");
$f->resume(1);
$f->resume([1,2]);
$f->resume(new stdclass);
?>
--EXPECTF--
string(6) "string"
int(1)
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
object(stdClass)#%d (0) {
}
