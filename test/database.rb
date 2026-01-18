describe 'database' do
  def run_script(commands)
    raw_output = nil 
    IO.popen("./a ramdb","r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end
    pipe.close_write
    raw_output = pipe.gets(nil)
    end 
    raw_output.split("\n")
  end

  # it 'keeps data after closing connection' do 
  #   result1= run_script([
  #     "insert 1 sujal gyawali",
  #     "./exit",
  #   ])
  #   expect(result1).to match_array([
  #     "db> Executed",
  #     "db> ",
  #   ])

  #   result2 = run_script([
  #     "select",
  #     "./exit",
  #   ])

  #   expect(result2).to match_array([
  #     "db> (1,sujal,gyawali)",
  #     "Executed",
  #     "db> ",
  #   ])
  # end 

  it 'allows printing out the structure of a one-node btree' do
    script =[3,1,2].map do |i|
      "insert #{i} user#{i} person#{i}@example.com"
    end 
    script<<".btree"
    script<<"./exit"
    result = run_script(script)
    expect(result).to match_array([
      "db> Executed",
      "db> Executed",
      "db> Executed",
      "db> Tree:",
      "leaf (size 3)",
      " -0: 3",
      " -1: 1",
      " -2: 2",
      "db> "
    ])
  end 
end