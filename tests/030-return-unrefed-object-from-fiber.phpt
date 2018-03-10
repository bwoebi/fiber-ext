--TEST--
Test can return an unreferenced object from fiber to main.
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber(function () {
    echo "CREATE OBJECT\n";
    return new class() {
        public function __destruct() {
            echo "DESTROYED\n";
        }
    };
});

echo "START\n";
$a = $f->start();
echo "DONE\n=> ";
$a = null;
echo "TERMINATED\n";

?>
--EXPECTF--
START
CREATE OBJECT
DONE
=> DESTROYED
TERMINATED
