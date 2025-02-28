import subprocess
import pytest
import os

def pytest_configure(config):
    if not os.path.isfile('./build/app.exe'):
        pytest.exit("Executable not found. Please compile the application first!", returncode=1)


valid_cases = [
    ("3 + 4", "7", "int"),
    ("10 - 2 * 3", "4", "int"),
    ("25 / 5", "5", "int"),
    ("(2 + 6) * 4", "32", "int"),
    ("2 + 8", "10", "int"),
    ("4 * 2 / 2", "4", "int")
]

error_cases = [
    ("3 + b", "", "int"),
    ("10 / 0", "", "int"),
    ("10 / 0", "", "float"),
    ("(3 + 5", "", "int"),
    ("3 + 5)", "", "int"),
]

def run_calculator(input_str, mode):
    cmd = ["./build/app.exe", mode]  
    proc = subprocess.Popen(
        cmd,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = proc.communicate(input=input_str + "\n")
    return stdout.strip(), stderr.strip()

@pytest.mark.parametrize("input_str,expected_output,mode", valid_cases)
def test_valid_expressions(input_str, expected_output, mode):
    output, error = run_calculator(input_str, mode)
    assert error == ""
    assert output == expected_output

@pytest.mark.parametrize("input_str,expected_output,mode", error_cases)
def test_invalid_expressions(input_str, expected_output, mode):
    output, error = run_calculator(input_str, mode)
    assert output == ""


def test_expression_with_spaces():
    output, error = run_calculator(" 3   +  (  4  * 2 )  ", "int")
    assert output == "11"


def test_precedence_of_operators():
    output, error = run_calculator("3 + 4/ 2", "int")
    assert output == "5"
    output, error = run_calculator("(3 + 4)/ 2", "int")
    assert output == "3"


def test_negative_calculations():
    output, error = run_calculator("0-7 - 6", "int")
    assert output == "-13"
