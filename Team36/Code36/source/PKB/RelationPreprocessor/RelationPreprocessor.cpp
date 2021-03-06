#include "RelationPreprocessor.h"

void RelationPreprocessor::reset() {
	is_fully_populated = false;
	int size = calculated_matrix.size();
	for (int i = 0; i < size; i++) {
		calculated_dfs_forward[i] = false;
		calculated_dfs_backward[i] = false;
		for (int j = 0; j < size; j++) {
			calculated_matrix[i][j] = false;
		}
	}
	cache = MonotypeRelationTable<StmtInfo>();
}

void RelationPreprocessor::setDFSForwardTrue(int index) {
	for (int i = 0; i < stmt_info_list.size(); i++) {
		calculated_matrix[index - 1][i] = true;
	}
	calculated_dfs_forward[index - 1] = true;
}

void RelationPreprocessor::setDFSBackwardTrue(int index) {
	for (int i = 0; i < stmt_info_list.size(); i++) {
		calculated_matrix[i][index - 1] = true;
	}
	calculated_dfs_backward[index - 1] = true;
}

bool RelationPreprocessor::isFullyPopulated() {
	return is_fully_populated;
}

const MonotypeRelationTable<StmtInfo>& RelationPreprocessor::getCache() {
	return cache;
}

bool RelationPreprocessor::isCacheEmpty() {
	return cache.isEmpty();
}

bool RelationPreprocessor::isCalculated(int key, int value) {
	return calculated_matrix[key][value];
}

bool RelationPreprocessor::isDFSForwardComputed(int index) {
	return calculated_dfs_forward[index - 1];
}

bool RelationPreprocessor::isDFSBackwardComputed(int index) {
	return calculated_dfs_backward[index - 1];
}