--TEST--
Include a file from within a running fiber.
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php

$f = new Fiber(function () {
    require __DIR__ . '/include/foo.php';

    return new Foo();
});

echo "START\n";
echo "=> ", get_class($f->start()), "\n";
echo "END\n";

?>
--EXPECT--
START
=> Foo
END
