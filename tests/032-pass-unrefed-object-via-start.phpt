--TEST--
Test can pass an unreferenced object into fiber via start().
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber(function ($a) {
    echo "YIELD\n";
    Fiber::yield();
    echo "COMPLETED\n=> ";
});

echo "START\n";
$f->start(new class() {
    public function __destruct() {
        echo "DESTROYED\n";
    }
});
echo "RESUME\n";
$f->resume();
echo "TERMINATED\n";

?>
--EXPECTF--
START
YIELD
RESUME
COMPLETED
=> DESTROYED
TERMINATED
