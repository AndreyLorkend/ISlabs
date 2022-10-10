extern "C"
int findNegativeCount(int length, int* array) {
	int negativeCount = 0;
	int index = length;
	for (int i = length-1; i >= 0; i--) {
		if (array[i] == 0) {
			index = i;
			break;
		}
	}

	for (int i = 0; i < index; i++) {
		if (array[i] < 0) {
			negativeCount++;
		}
	}

	return negativeCount;
}

extern "C"
int findPositiveArithmeticMean(int length, int* array) {
	int sum = 0;
	int count = 0;
	for (int i = 0; i < length; i++) {
		if (array[i] > 0) {
			sum += array[i];
			count++;
		}
	}

	if (count != 0) {
		sum = int(sum / count);
	}

	return sum;
}