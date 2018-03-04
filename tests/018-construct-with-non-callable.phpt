--TEST--
tests Fiber for error memory leak
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber;
$f->resume();
?>
--EXPECTF--
Fatal error: Uncaught ArgumentCountError: Fiber::__construct() expects at least 1 parameter, 0 given in %s/tests/018-construct-with-non-callable.php:2
Stack trace:
#0 %s/tests/018-construct-with-non-callable.php(2): Fiber->__construct()
#1 {main}
  thrown in %s/tests/018-construct-with-non-callable.php on line 2
