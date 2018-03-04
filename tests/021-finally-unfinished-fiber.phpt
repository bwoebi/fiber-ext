--TEST--
Test finally blocks are executed in unfinished fiber
--SKIPIF--
<?php if (!extension_loaded('fiber')) echo 'skip'; ?>
--FILE--
<?php

$f = new Fiber(function () {
    try {
        echo "yielding - ";
        Fiber::yield();
        echo "not reached";
    } finally {
        echo "finally";
    }
});

$f->start();

?>
--EXPECTF--
yielding - finally
