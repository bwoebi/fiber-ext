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
function bar()
{
    $a = null;
    Fiber::yield();
    $a->foo();
}
function foo()
{
    bar();
}
$f = new Fiber(function () {
    foo();
});

$f->start();

try {
    $f->resume();
} catch (Error $e) {
    echo $e->getMessage();
}

?>
--EXPECTF--
Call to a member function foo() on null
