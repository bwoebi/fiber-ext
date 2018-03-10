--TEST--
Test can yield an unreferenced object from fiber to main.
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber(function () {
    echo "YIELD\n";
    Fiber::yield(new class() {
        public function __destruct() {
            echo "DESTROYED\n";
        }
    });
    echo "COMPLETED\n";
});

echo "START\n";
$a = $f->start();
echo "RESUME\n";
$f->resume();
echo "DONE\n=> ";
$a = null;
echo "TERMINATED\n";

?>
--EXPECTF--
START
YIELD
RESUME
COMPLETED
DONE
=> DESTROYED
TERMINATED
