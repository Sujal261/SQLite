describe 'database' do 
   def run_script(commands)
    raw_output = nil 
    IO.popen("./a sujaldb","r+") do |pipe|
      commands.each do |command|
        pipe.puts command

      end

      pipe.close_write

      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it "elements aranged in sorted order" do
    result = run_script([
      'insert 3 Sujal Gyawali',
      "insert 1 Ram Hari",
      "select",
      "./exit",
    ])

    expect(result).to match_array([
      "db> Executed",
      "db> Executed",
      "(1,Ram,Hari)",
      "db> (3,Sujal,Gyawali)",
      "Executed",
      "db> "
    ])
  end 
end