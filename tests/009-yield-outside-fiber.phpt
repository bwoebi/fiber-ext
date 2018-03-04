--TEST--
tests Fiber::yield out of fiber internal call
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
	echo 'skip';
}
?>
--FILE--
<?php
try {
    Fiber::yield();
} catch (Error $e) {
    echo $e->getMessage();
}
?>
--EXPECTF--
Cannot yield from outside a fiber
