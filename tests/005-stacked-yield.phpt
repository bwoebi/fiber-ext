--TEST--
tests Fiber stackful await
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
    echo "before\n";
    Fiber::yield();
    echo "after\n";
}
$f = new Fiber(function () {
    foo();
    Fiber::yield();
    echo "bye\n";
});
$f->start();
echo "await\n";
$f->resume();
echo "await2\n";
$f->resume();
?>
--EXPECTF--
before
await
after
await2
bye
