--TEST--
tests Fiber status
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
	echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber(function () {
    Fiber::yield();
});

var_dump($f->status() == Fiber::STATUS_INIT);
$f->start();
var_dump($f->status() == Fiber::STATUS_SUSPENDED);
$f->resume();
var_dump($f->status() == Fiber::STATUS_FINISHED);
try {
    $f->resume();
} catch (Error $e) {
    echo $e->getMessage(), PHP_EOL;
}
var_dump($f->status() == Fiber::STATUS_FINISHED);

$f = new Fiber(function () {
    throw new Exception;
});
try {
    $f->start();
} catch (Exception $e) {
}
var_dump($f->status() == Fiber::STATUS_DEAD);
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
Non-suspended Fiber cannot be resumed
bool(true)
bool(true)
