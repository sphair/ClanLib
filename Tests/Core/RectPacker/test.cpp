#include <ClanLib/core.h>
using namespace clan;

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Testing fail_if_full:" << std::endl;
		RectPacker packer(Size(100,100), RectPacker::fail_if_full);
		RectPacker::AllocatedRect allocation1 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation2 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation3 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation4 = packer.add(Size(50,50));

		std::cout << "Expected: Allocation OK" << std::endl;
		std::cout << "allocation1: Id: " << allocation1.group_index << " Pos: " << allocation1.rect.left << ", " << allocation1.rect.top << std::endl;
		std::cout << "allocation2: Id: " << allocation2.group_index << " Pos: " << allocation2.rect.left << ", " << allocation2.rect.top << std::endl;
		std::cout << "allocation3: Id: " << allocation3.group_index << " Pos: " << allocation3.rect.left << ", " << allocation3.rect.top << std::endl;
		std::cout << "allocation4: Id: " << allocation4.group_index << " Pos: " << allocation4.rect.left << ", " << allocation4.rect.top << std::endl;
	}
	catch (Exception &e)
	{
		std::cout << "Did not expect: Allocation failed: " << e.message.c_str() << std::endl;		
	}

	try
	{
		std::cout << std::endl << "Testing create_new_group:" << std::endl;

		RectPacker packer(Size(100,100), RectPacker::create_new_group);
		RectPacker::AllocatedRect allocation1 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation2 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation3 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation4 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation5 = packer.add(Size(50,50));

		std::cout << "Expected: Allocation OK" << std::endl;
		std::cout << "allocation1: Id: " << allocation1.group_index << " Pos: " << allocation1.rect.left << ", " << allocation1.rect.top << std::endl;
		std::cout << "allocation2: Id: " << allocation2.group_index << " Pos: " << allocation2.rect.left << ", " << allocation2.rect.top << std::endl;
		std::cout << "allocation3: Id: " << allocation3.group_index << " Pos: " << allocation3.rect.left << ", " << allocation3.rect.top << std::endl;
		std::cout << "allocation4: Id: " << allocation4.group_index << " Pos: " << allocation4.rect.left << ", " << allocation4.rect.top << std::endl;
		std::cout << "allocation5: Id: " << allocation5.group_index << " Pos: " << allocation5.rect.left << ", " << allocation5.rect.top << std::endl;
	}
	catch (Exception &e)
	{
		std::cout << "Did not expect: Allocation failed: " << e.message.c_str() << std::endl;		
	}

	try
	{
		std::cout << std::endl << "Testing failing fail_if_full because of full:" << std::endl;

		RectPacker packer(Size(100,100), RectPacker::fail_if_full);
		RectPacker::AllocatedRect allocation1 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation2 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation3 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation4 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation5 = packer.add(Size(50,50));

		std::cout << "Did not expect: Allocation OK" << std::endl;
	}
	catch (Exception &e)
	{
		std::cout << "Expected: " << e.message.c_str() << std::endl;		
	}

	try
	{
		std::cout << "Testing failing fail_if_full because of too large size:" << std::endl;

		RectPacker packer(Size(100,100), RectPacker::fail_if_full);
		RectPacker::AllocatedRect allocation5 = packer.add(Size(200,50));

		std::cout << "Did not expect: Allocation OK" << std::endl;
	}
	catch (Exception &e)
	{
		std::cout << "Expected: " << e.message.c_str() << std::endl;		
	}

	try
	{
		std::cout << "Testing failing create_new_group because of too large size:" << std::endl;

		RectPacker packer(Size(100,100), RectPacker::create_new_group);
		RectPacker::AllocatedRect allocation1 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation2 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation3 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation4 = packer.add(Size(50,50));
		RectPacker::AllocatedRect allocation5 = packer.add(Size(200,50));

		std::cout << "Did not expect: Allocation OK" << std::endl;
	}
	catch (Exception &e)
	{
		std::cout << "Expected: " << e.message.c_str() << std::endl;		
	}
	return 0;
}

