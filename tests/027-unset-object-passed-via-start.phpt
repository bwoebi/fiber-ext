--TEST--
Test refcount of objects passed via start() is preserved.
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

$f = new Fiber(function ($a) {
    echo "UNSET\n";
    $a = null;
    echo "COMPLETED\n";
    return $a;
});

echo "START\n";
$f->start($a);
echo "DONE\n=> ";
$a = null;
echo "TERMINATED\n";

?>
--EXPECTF--
START
UNSET
COMPLETED
DONE
=> DESTROYED
TERMINATED
