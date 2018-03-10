--TEST--
Call a method of an object that is not referenced from the main execution as fiber.
--SKIPIF--
<?php
if (!extension_loaded('fiber')) {
    echo 'skip';
}
?>
--FILE--
<?php
$f = new Fiber([new class() {
    private $b = 3;
    public function __destruct() {
        echo "DESTROYED\n";
    }
    public function test($a) {
        echo "YIELD\n";
        $b = Fiber::yield($this->b);
        echo "COMPLETED\n=> ";
        return $a + $b;
    }
}, 'test']);

echo "START\n";
$b = $f->start(1);
echo "RESUME\n";
$c = $f->resume($b - 1);
echo "RESULT: $c\n";
echo "TERMINATED\n";

?>
--EXPECTF--
START
YIELD
RESUME
COMPLETED
=> DESTROYED
RESULT: 3
TERMINATED
