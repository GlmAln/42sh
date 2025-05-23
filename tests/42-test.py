import unittest
import subprocess

class Test42sh_simpleecho(unittest.TestCase):
    def test_echo_input_as_string(self):
        command = "./42sh -c 'echo Input as string'"
        expected_output = "Input as string\n"  
        result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        self.assertEqual(result.stdout, expected_output)

    def test_echo_input_from_sh(self):
        command = "./42sh script.sh"
        expected_output = "Input as string"  
        result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        self.assertEqual(result.stdout, expected_output)
    
    def test_echo_input_stdin(self):
        input_command = "echo through stdin"
        command = "./42sh"
        expected_output = "through stdin\n"
        result = subprocess.run(command, shell=True, input=input_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        self.assertEqual(result.stdout, expected_output)

if __name__ == '__main__':
    unittest.main()