import java.util.Random

class Statistics {
	def count
	def index
	def total
	def values = []

	Statistics(length) {
		count = length
		(0..count - 1).each {
			values << 0
		}
		index = count
		total = 0
	}

	def incrementIndex() {
		if (index++ >= count - 1) {
			index = 0
		}
	}

	def addValue(value) {
		incrementIndex()
		total -= values[index]
		values[index] = value
		total += value
	}

	def sum() {
		return total
	}

	def average() {
		return total / count
	}

	def stddev() {
		def avg = average()
		def var = 0
		(0..count-1).each {
			var += (values[it] - avg)**2
		}
		return Math.sqrt(var / (count - 1))
	}

	def printValues() {
		(0..count-1).each {
			printf("%5d", values[it])
			if (it == index) print("*")
			else print(" ")
		}
		printf(" [ S: %5d  M: %7.2f  D: %7.2f ]\n", sum(), average(), stddev())
	}

	def printDeviation() {
		print(" ")
		(0..count-1).each {
			def var = (average() - values[it]) / stddev()
			printf("% 1.1f", var)
			if (var >= 2 || var <= -2) print(" !")
			else print("  ")
		}
		print("\n\n")	
	}

	def testValue(value) {
		def var = (average() - value) / stddev()
		printf("%5d : % 1.1f", value, var)
		if (var >= 2 || var <= -2) print(" !")
		else print("  ")
		printf("   [ S: %5d  M: %7.2f  D: %7.2f ]\n", sum(), average(), stddev())
		addValue(value)
	}
}

Random rand = new Random()
Statistics statistics = new Statistics(30)
// statistics.printValues()
(0..30).each {
	statistics.addValue(rand.nextInt(200))
}
(0..50).each {
	statistics.testValue(rand.nextInt(200))
	// statistics.printValues()
	// statistics.printDeviation()
}
