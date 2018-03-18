--TEST--
test too few arguments for start
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber(function ($a) {
    Fiber::yield($a);
});

try {
    $f->start();
} catch (ArgumentCountError $e) {
    echo $e->getMessage();
}
?>
--EXPECTF--
Too few arguments to function {closure}(), 0 passed and exactly 1 expected
