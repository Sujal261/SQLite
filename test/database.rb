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

  it 'keeps data after closing connection' do 
    result1= run_script([
      "insert 1 sujal gyawali",
      "./exit",
    ])
    expect(result1).to match_array([
      "db> Executed",
      "db> ",
    ])

    result2 = run_script([
      "select",
      "./exit",
    ])

    expect(result2).to match_array([
      "db> (1,sujal,gyawali)",
      "Executed",
      "db> ",
    ])
  end 
end