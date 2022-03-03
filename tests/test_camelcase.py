from yascc.camelcase import decamelize


def test_decamelize(camelized_dict: dict, decamelized_dict: dict) -> None:
    got = decamelize(camelized_dict)
    assert got == decamelized_dict
