--TEST--
tests Fiber for exception
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
function foo()
{
    Fiber::yield();
    throw new Exception('FOOBAR!');
}

function bar()
{
    Fiber::yield();
    foo();
}

$f = new Fiber('bar');

$f->start();
$f->resume();

try {
    $f->resume();
} catch (\Exception $e) {
    echo $e->getMessage();
}
?>
--EXPECTF--
FOOBAR!
