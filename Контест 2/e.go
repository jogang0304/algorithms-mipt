package main

import "fmt"

type Pair[T any] struct {
	First  T
	Second T
}

func Equal(a, b Pair[int]) bool {
	return a.First == b.First
}

func SortCondition(a, b Pair[int]) bool {
	return a.First <= b.First
}

func Merge(values *[]Pair[int], answers *[]int, left, right int) []Pair[int] {
	medium := (left + right) / 2
	left_part_index := left
	right_part_index := medium
	sorted_array := make([]Pair[int], right-left)
	for left_part_index < medium || right_part_index < right {
		if left_part_index < medium && right_part_index < right {
			if SortCondition((*values)[left_part_index], (*values)[right_part_index]) {
				(*answers)[(*values)[left_part_index].Second] += right_part_index - medium
				sorted_array = append(sorted_array, (*values)[left_part_index])
				left_part_index++
			} else {
				sorted_array = append(sorted_array, (*values)[right_part_index])
				right_part_index++
			}
		} else if left_part_index < medium {
			(*answers)[(*values)[left_part_index].Second] += right_part_index - medium
			sorted_array = append(sorted_array, (*values)[left_part_index])
			left_part_index++
		} else {
			sorted_array = append(sorted_array, (*values)[right_part_index])
			right_part_index++
		}
	}
	return sorted_array
}

func MergeSort(values *[]Pair[int], answers *[]int, left, right int) {
	if right-left == 2 {
		if !SortCondition((*values)[left], (*values)[right-1]) {
			(*answers)[(*values)[left].Second]++
			(*values)[left], (*values)[right-1] = (*values)[right-1], (*values)[left]
		}
	} else if right-left > 2 {
		MergeSort(values, answers, left, (left+right)/2)
		MergeSort(values, answers, (left+right)/2, right)
		sorted_array := Merge(values, answers, left, right)
		for i := left; i < right; i++ {
			(*values)[i] = sorted_array[i-left]
		}
	}
}

func main() {
	var amount_of_elements int
	fmt.Scan(&amount_of_elements)
	var values = make([]Pair[int], amount_of_elements)
	index := 0
	for e := range values {
		fmt.Scan(&values[e].First)
		values[e].Second = index
		index++
	}
	var answers = make([]int, amount_of_elements)
	MergeSort(&values, &answers, 0, amount_of_elements)
	for e := range answers {
		fmt.Print(answers[e], " ")
	}
}
