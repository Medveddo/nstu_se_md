import kotlin.math.pow
import kotlin.math.sqrt
import kotlin.random.Random
import java.math.RoundingMode

class GeoCoordinate(val latitude: Double, val longitude: Double) : CustomTypeTrait {

    override val typeName: String
        get() = "GeoCoordinate"

    override val typeExampleRepresentation: String
        get() = "1.5|2.8"

    override fun createRandomInstance(): CustomTypeTrait {
        val latitude = (Random.nextDouble() * 200 - 100).toBigDecimal().setScale(2, RoundingMode.HALF_UP).toDouble()
        val longitude = (Random.nextDouble() * 200 - 100).toBigDecimal().setScale(2, RoundingMode.HALF_UP).toDouble()

        return GeoCoordinate(latitude, longitude)
    }

    override fun toString(): String {
        return "Lat: $latitude, Long: $longitude"
    }

    override fun serializeToString(): String {
        return "$latitude|$longitude"
    }

    override fun deserializeFromString(str: String): CustomTypeTrait? {
        val parts = str.split('|')
        return when {
            parts.size == 2 -> {
                try {
                    val latitude = parts[0].toDouble()
                    val longitude = parts[1].toDouble()
                    GeoCoordinate(latitude, longitude)
                } catch (e: NumberFormatException) {
                    null
                }
            }
            else -> null
        }
    }

    override fun compare(other: CustomTypeTrait): Int {
        return when (other) {
            is GeoCoordinate -> {
                when {
                    latitude < other.latitude -> -1
                    latitude > other.latitude -> 1
                    else -> 0
                }
            }
            else -> throw IllegalArgumentException("Cannot compare GeoCoordinate with a different type")
        }
    }
}
