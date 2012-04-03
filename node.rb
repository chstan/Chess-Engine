class Node
  attr_accessor :value, :children
  
  def initialize value, children=[]
    @children = children
    @value = value
  end
  
  def to_s indent=0
    value_s = @value.to_s
    sub_indent = indent + value_s.length
    value_s + @children.map { |child| " - " + child.to_s(sub_indent + 3) }.join("\n" + ' '*sub_indent)
  end
end