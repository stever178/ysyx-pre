#!/bin/bash
# analyze_cache.sh - 分析所有 cachegrind 结果文件
# 用法: ./tests/analyze_cache.sh

set -e
echo "=== Cachegrind Analysis Start ==="

OUTPUT_DIR="build/analysis_cachegrind"
mkdir -p $OUTPUT_DIR

FILES_DIR="build/cachegrind"
if [ ! -d "$FILES_DIR" ]; then
    echo "No cachegrind files found in $FILES_DIR"
    exit 1
fi

for cache_file in ${FILES_DIR}/*.out; do
    [ -e "$cache_file" ] || continue
    
    target="${cache_file#${FILES_DIR}/}"
    target="${target%.out}"
    
    echo "--- Analyzing: $target ---"
    
    src_base="${target%_tests}"
    src_file="src/lcthw/${src_base}.c"
    
    # 检查源文件是否存在
    if [ ! -f "$src_file" ]; then
        echo "  WARNING: Source file not found: $src_file"
        src_file=""  # 不指定源文件进行通用分析
    fi
    
    # 构建输出文件名
    output_file="${OUTPUT_DIR}/${target}.txt"
    
    # 执行 callgrind_annotate 分析
    echo "  Input:  $cache_file"
    echo "  Source: ${src_file:-N/A (generic analysis)}"
    echo "  Output: $output_file"
    
    if [ -n "$src_file" ]; then
        callgrind_annotate --tree=both \
            "$call_file" "$src_file" > "$output_file"
    else
        callgrind_annotate --tree=both \
            "$call_file" > "$output_file"
    fi

    echo "  ✓ Analysis saved to $output_file"
    echo ""
done

echo "=== Cachegrind Analysis Complete ==="
echo "Results saved to build/analysis_cachegrind_*.txt"
echo ""
