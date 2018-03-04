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
$f->start();
?>
--EXPECTF--
Fatal error: Uncaught ArgumentCountError: Too few arguments to function {closure}(), 0 passed and exactly 1 expected in %s/tests/020-start-with-too-few-arguments.php:2
Stack trace:
#0 [internal function]: {closure}()
#1 %s/tests/020-start-with-too-few-arguments.php(5): Fiber->start()
#2 {main}
  thrown in %s/tests/020-start-with-too-few-arguments.php on line 2
