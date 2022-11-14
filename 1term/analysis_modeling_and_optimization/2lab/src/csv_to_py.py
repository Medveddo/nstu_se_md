import csv
from typing import List

with open("data.csv", "r", encoding="utf-8") as file:
    reader = csv.reader(file, delimiter=";")
    x_values: List[int] = []
    y_values: List[float] = []
    next(reader)  # skip header row
    for row in reader:
        print(row)
        x_values.append(int(row[0]))
        y_value = row[1].replace(",", ".")
        y_value = float(y_value)
        print(y_value)
        if y_value < 100:
            y_value *= 1000
        y_values.append(y_value)

    print(f"{x_values=}")
    print(f"{y_values=}")

# result
x_values=[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139]
y_values=[0.0, 252.0, 482.0, 737.0, 974.0, 1190.0, 1396.0, 1617.0, 1805.0, 1988.0, 2168.0, 2310.0, 2471.0, 2618.0, 2763.0, 2892.0, 3004.0, 3123.0, 3255.0, 3345.0, 3443.0, 3549.0, 3637.0, 3719.0, 3785.0, 3858.0, 3931.0, 3987.0, 4054.0000000000005, 4113.0, 4163.0, 4203.0, 4253.0, 4289.0, 4322.0, 4359.0, 4404.0, 4443.0, 4476.0, 4511.0, 4535.0, 4559.0, 4584.0, 4607.0, 4633.0, 4653.0, 4668.0, 4690.0, 4708.0, 4721.0, 4734.0, 4758.0, 4769.0, 4777.0, 4792.0, 4810.0, 4824.0, 4840.0, 4855.0, 4864.0, 4872.0, 4878.0, 4886.0, 4891.0, 4899.0, 4909.0, 4919.0, 4923.0, 4929.0, 4935.0, 4937.0, 4942.0, 4949.0, 4954.0, 4957.0, 4961.0, 4966.0, 4970.0, 4973.0, 4974.0, 4976.0, 4977.0, 4978.0, 4979.0, 4980.0, 4983.0, 4983.0, 4984.0, 4984.0, 4984.0, 4984.0, 4988.0, 4988.0, 4989.0, 4989.0, 4989.0, 4989.0, 4990.0, 4990.0, 4991.0, 4991.0, 4991.0, 4991.0, 4992.0, 4992.0, 4993.0, 4993.0, 4993.0, 4993.0, 4993.0, 4994.0, 4994.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4995.0, 4996.0, 4996.0, 4996.0, 4996.0, 4996.0, 4996.0, 4997.0, 4997.0, 4997.0, 4998.0, 4998.0, 4999.0, 4999.0, 4999.0, 5000.0, 5000.0]
