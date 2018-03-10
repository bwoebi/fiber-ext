--TEST--
Test refcount of objects passed via resume() is preserved.
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$a = new class() {
    public function __destruct() {
        echo "DESTROYED\n";
    }
};

$f = new Fiber(function () {
    echo "YIELD\n";
    $a = Fiber::yield();
    echo "YIELDED\n";
    $a = null;
    echo "COMPLETED\n";
    return $a;
});

echo "START\n";
$f->start();
echo "RESUME\n";
$f->resume($a);
echo "DONE\n=> ";
$a = null;
echo "TERMINATED\n";

?>
--EXPECTF--
START
YIELD
RESUME
YIELDED
COMPLETED
DONE
=> DESTROYED
TERMINATED
