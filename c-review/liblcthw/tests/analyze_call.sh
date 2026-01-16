#!/usr/bin/env bash
# analyze_call.sh - 分析所有 callgrind 结果文件
# 用法: ./tests/analyze_call.sh

set -e
echo "=== Callgrind Analysis Start ==="

OUTPUT_DIR="build/analysis_callgrind"
mkdir -p $OUTPUT_DIR

FILES_DIR="build/callgrind"
if [ ! -d "$FILES_DIR" ]; then
    echo "No callgrind files found in $FILES_DIR"
    exit 1
fi

for call_file in ${FILES_DIR}/*.out; do
    [ -e "$call_file" ] || continue
    
    target="${call_file#${FILES_DIR}/}"
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
    echo "  Input:  $call_file"
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

echo "=== Callgrind Analysis Complete ==="
echo "Results saved to build/analysis_callgrind_*.txt"
echo ""
