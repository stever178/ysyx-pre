"""
Docstring for ex.ex33
merge_sort_bottom_up
"""


def merge(left, right):
    """
    Docstring for merge

    :param left: Description
    :param right: Description
    """
    result = []
    i, j = 0, 0
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result


def merge_sort_bottom_up(arr_input):
    """
    Docstring for merge_sort_bottom_up

    :param arr_input: Description
    """
    n = len(arr_input)
    size = 1  # 当前归并的子数组大小

    while size < n:
        for left in range(0, n, 2 * size):
            mid = min(left + size, n)
            right = min(left + 2 * size, n)
            # notice: [left, right)
            if mid < right:  # 避免空合并
                arr_input[left:right] = merge(arr_input[left:mid], arr_input[mid:right])
        size *= 2
    return arr_input


# 示例调用
arr = [5, 2, 4, 7, 1, 3, 2, 6]
sorted_arr = merge_sort_bottom_up(arr)
print(sorted_arr)
