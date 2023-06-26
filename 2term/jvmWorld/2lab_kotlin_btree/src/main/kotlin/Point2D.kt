import kotlin.math.pow
import kotlin.math.sqrt
import kotlin.random.Random
import java.math.RoundingMode

class Point2D(val x: Double, val y: Double) : CustomTypeTrait {

    override val typeName: String
        get() = "Point2D"

    override val typeExampleRepresentation: String
        get() = "0.5|7.8"

    override fun createRandomInstance(): Point2D {
        val x = (Random.nextDouble() * 20 - 10).toBigDecimal().setScale(2, RoundingMode.HALF_UP).toDouble()
        val y = (Random.nextDouble() * 20 - 10).toBigDecimal().setScale(2, RoundingMode.HALF_UP).toDouble()
        return Point2D(x, y)
    }

    override fun toString(): String {
        return "X: $x, Y: $y"
    }

    override fun serializeToString(): String {
        return "$x|$y"
    }

    override fun deserializeFromString(str: String): Point2D? {
        val parts = str.split('|')
        return if (parts.size == 2) {
            try {
                val x = parts[0].toDouble()
                val y = parts[1].toDouble()
                Point2D(x, y)
            } catch (e: NumberFormatException) {
                null
            }
        } else {
            null
        }
    }

    val distanceFromOrigin: Double
        get() = sqrt(x.pow(2) + y.pow(2))

    override fun compare(other: CustomTypeTrait): Int {
        return when (other) {
            is Point2D -> {
                val distanceA = this.distanceFromOrigin
                val distanceB = other.distanceFromOrigin
                distanceA.compareTo(distanceB)
            }
            else -> throw IllegalArgumentException("Cannot compare Point with a different type")
        }
    }
}
