import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertNotNull
import org.junit.jupiter.api.Test
import kotlin.random.Random

class BinaryTreeTest {

    @Test
    fun testInsertion() {
        val tree = BinaryTree<GeoCoordinate>()
        val geo1 = GeoCoordinate(0.0, 0.0)
        val geo2 = GeoCoordinate(1.0, 0.0)
        val geo3 = GeoCoordinate(-1.0, 0.0)
        tree.insert(geo1)
        tree.insert(geo2)
        tree.insert(geo3)
        val root = tree.getParentNode()
        assertNotNull(root)
        assertEquals(0, geo1.compare(root?.value as CustomTypeInterface))
        assertEquals(0, geo2.compare(root.right?.value as CustomTypeInterface))
        assertEquals(0, geo3.compare(root.left?.value as CustomTypeInterface))
    }

    private fun compareTrees(vararg trees: BinaryTree<GeoCoordinate>): Boolean {
        if (trees.size < 2) {
            return true
        }

        val iterators = trees.map { it.depthFirstIterator() }

        while (iterators.all { it.hasNext() }) {
            val elements = iterators.map { it.next().first }

            val firstElement = elements[0]

            for (i in 1 until elements.size) {
                val otherElement = elements[i]

                if (firstElement.compare(otherElement) != 0) {
                    return false
                }
            }
        }

        return !iterators.any { it.hasNext() }
    }

    @Test
    fun testBalancing() {
        val tree = BinaryTree<GeoCoordinate>()
        val coordinates = arrayOf(
            GeoCoordinate(-46.71, 7.61),
            GeoCoordinate(-49.20, -15.80),
            GeoCoordinate(-42.24, -2.04),
            GeoCoordinate(24.46, 80.96),
            GeoCoordinate(-33.00, -63.76),
            GeoCoordinate(-23.81, 123.39),
            GeoCoordinate(32.30, 98.81),
            GeoCoordinate(-19.48, -7.25),
            GeoCoordinate(-63.20, -65.03),
            GeoCoordinate(60.30, -134.52),
            GeoCoordinate(60.30, -134.52),
            GeoCoordinate(60.30, -134.52),
            GeoCoordinate(60.30, -134.52),
            GeoCoordinate(60.30, -134.52),
        )
        for (coordinate in coordinates) {
            tree.insert(coordinate)
        }
        tree.rebalance()
        val tree2 = BinaryTree<GeoCoordinate>()
        val sortedCoordinates = coordinates.sortedWith { coordinate1, coordinate2 ->
            coordinate1.compare(coordinate2)
        }
        for (coordinate in sortedCoordinates) {
            tree2.insert(coordinate)
        }
        tree2.rebalance()
        val tree3 = BinaryTree<GeoCoordinate>()
        val sortedCoordinatesReversed = coordinates.sortedWith { coordinate1, coordinate2 ->
            coordinate2.compare(coordinate1)
        }
        for (coordinate in sortedCoordinatesReversed) {
            tree3.insert(coordinate)
        }
        tree3.rebalance()
        assertEquals(true, compareTrees(tree, tree2, tree3))
    }

    private fun getDataSet(): Array<GeoCoordinate> {
        val random = Random(42)
        return Array(4000000) {
            GeoCoordinate(
                random.nextDouble(-90.0, 90.0),
                random.nextDouble(-180.0, 180.0)
            )
        }
    }

    @Test
    fun testPerformance() {
        val coordinates = getDataSet()
        val tree = BinaryTree<GeoCoordinate>()
        for (coordinate in coordinates) {
            tree.insert(coordinate)
        }
        tree.rebalance()
    }
}