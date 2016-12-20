import matplotlib.pyplot as plt

xlist = [i for i in range(100, 1000, 100)]	# This is a list of number of find operations (x axis)
xlist = xlist + [i for i in range(1000, 10001, 1000)]

# Fill these lists. The length of these lists and xlist should be the same.

ylistbst = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]	 				# list of time values of find operations in a splay tree
ylistlist = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20] 					# list of time values of find operations in a splay tree

# Create a plot
plt.xlabel("number of find operations")
plt.ylabel("time")
plt.title("Performance of find operation in sorted linked list and splay trees")

plt.plot(xlist, ylistbst, 'b', label = "splay trees")
plt.plot(xlist, ylistlist, 'r', label = "sorted linked list")
plt.legend(loc = "upper left")

# Draw the plot
plt.show()
